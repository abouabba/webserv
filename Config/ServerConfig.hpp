#pragma once
#include <string>
#include <vector>
#include <map>
#include "LocationConfig.hpp"

struct ServerConfig {
    int listen;
    std::string host;
    std::string server_name;
    std::string root;
    std::vector<LocationConfig> locations;

    ServerConfig() : listen(-1) {}
};

ServerConfig& findServer(std::vector<ServerConfig>& servers, const std::string& host, int port);