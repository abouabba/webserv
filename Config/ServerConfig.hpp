#pragma once
#include <string>
#include <vector>
#include <map>
#include "LocationConfig.hpp"

struct ServerConfig {
    int port;
    std::string host;
    std::string server_name;
    std::string root;
    std::vector<LocationConfig> locations;

    ServerConfig() : port(0) {}
};
