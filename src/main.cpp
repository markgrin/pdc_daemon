#include <iostream>

#include <boost/program_options.hpp>

#include "conf.hpp"
#include "server.hpp"

int main (int argc, char** argv) {
    namespace po = boost::program_options;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("conf", po::value<std::string>(), "path to configuration json file")
        ;
        
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        
        pdc::config config;
        if (!vm.count("conf")) {
            std::cout << "Using default config\n";
        } else {
            config = pdc::read_config(vm["conf"].as<std::string>());
        }
        pdc::start(std::move(config));
    } catch (std::exception& e) {
        std::cout << "Caught exception:\n" << e.what() << "\n";
    } catch (...) {
        std::cout << "Caught unknown exception\n";
    }
    return 0;
}

