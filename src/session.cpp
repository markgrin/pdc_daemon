#include "session.hpp"

namespace {

/**
 * Helper function to parse uri
 * @param string string with uri
 * @param command out argument, command from uri.
 * @param args out argument, args of uri
 */
void parse_args(std::string string, std::string& command, std::map<std::string, std::string>& args) {
    if (string.empty())
        return ;
    if (string[0] != '/')
        return ;
    auto start = string.find("?");
    command = string.substr(1, start == std::string::npos ? start : start - 1);
    while (start != std::string::npos) {
        string.erase(0, start + 1);
        auto equal = string.find("=");
        if (equal == std::string::npos)
            break ;
        auto end   = string.find("&");
        args[string.substr(0, equal)] = string.substr(equal + 1, end - equal - 1);
        start = string.find("&");
    }
}

} // namespace

namespace pdc {

std::string session_manager::action(std::string string) {
    try {
        std::string command;
        std::map <std::string, std::string> args;
        parse_args(std::move(string), command, args);
        if (command == "begin") {
            std::lock_guard<std::mutex> guard(lock_);
            storage_.emplace(std::move(args["session"]), std::stoull(args["stat_size"]));

        } else if (command == "end") {
            if (args.find("session") == args.end())
                return "{ \"error\" : \"bad_input\"}";

            std::lock_guard<std::mutex> guard(lock_);
            auto it = storage_.find(args["session"]);
            if (it == storage_.end())
                return "{ \"error\" : \"bad_input\"}";
            {
                std::lock_guard<std::mutex> guard(it->second.lock_); // guarantee that session is not locked
            }
            storage_.erase(args["session"]);
        } else if (command == "add") {
            if (args.find("session") == args.end())
                return "{ \"error\" : \"bad_input\"}";

            std::unique_lock<std::mutex> master_guard(lock_);
            auto it = storage_.find(args["session"]);
            if (it == storage_.end())
                return "{ \"error\" : \"bad_input\"}";
            std::lock_guard<std::mutex> guard(it->second.lock_);
            master_guard.unlock();
            it->second.add(std::stoull(args["setup"]), std::stoull(args["talk"]));
        } else if (command == "get") {
            if (args.find("session") == args.end())
                return "{ \"error\" : \"bad_input\"}";

            std::unique_lock<std::mutex> master_guard(lock_);
            auto it = storage_.find(args["session"]);
            if (it == storage_.end())
                return "{ \"error\" : \"bad_input\"}";
            std::lock_guard<std::mutex> guard(it->second.lock_);
            master_guard.unlock(); // next call is time consuming, but only session lock is used
            auto result = it->second.get(std::stoull(args["free"]), std::stoull(args["incoming"]));
            return "{ \"call\" : \"" + std::to_string(result) + "\" }";
        } else {
            return "{ \"error\" : \"bad_input\"}";
        }
        return "{ \"success\" : \"true\"}";
    } catch (...) {
    }
    return "{ \"error\" : \"bad_input\"}";
}


} // namespace pdc