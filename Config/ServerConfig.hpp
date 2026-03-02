#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include "LocationConfig.hpp"

class ServerConfig {
    public:
        int listen;
        std::string host;
        std::string root;
        std::vector<LocationConfig> locations;

    ServerConfig() : listen(-1) {}

};


#endif