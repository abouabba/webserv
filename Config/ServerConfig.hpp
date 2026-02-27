#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include "LocationConfig.hpp" // هنا مسموح

struct ServerConfig {
    int listen;
    std::string host;
    std::string root;
    std::vector<LocationConfig> locations;

    ServerConfig() : listen(-1) {}
};

ServerConfig& findServer(std::vector<ServerConfig>& servers,
                         const std::string& host,
                         int port);

#endif