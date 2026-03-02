#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <string>
#include <vector>

class ServerConfig;

class LocationConfig {
    public:
        std::string path;
        std::string root;
        std::vector<std::string> methods;
        bool autoindex;
};

#endif