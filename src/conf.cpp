#include "conf.hpp"

#include <json/json.h>

#include <fstream>
#include <iostream>

namespace {

void get_int(int& destination, Json::Value& source) {
    if (source.isInt())
        destination = source.asInt();
}

void get_uint(std::size_t& destination, Json::Value& source) {
    if (source.isUInt())
        destination = source.asUInt();
}

} // namespace

namespace pdc {

config read_config (const std::string& filename) {
    config conf;
    Json::Value root;
    std::ifstream file(filename, std::ios::in);
    if (!file)
        throw std::runtime_error("Error opening config file");

    Json::CharReaderBuilder builder;
    std::string string;
    if (!Json::parseFromStream(builder, file, &root, &string)) {
        std::cout << string;
        throw std::runtime_error("Json config reading error");
    }

    get_int(conf.port, root["port"]);
    get_uint(conf.threads, root["threads"]);

    return config();
}

} //namespace pdc
