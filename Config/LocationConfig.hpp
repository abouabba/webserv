#pragma once
#include <string>
#include <vector>

struct LocationConfig {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    bool autoindex;

    LocationConfig() : autoindex(false) {}
};

LocationConfig* findLocation(ServerConfig& server, const std::string& uri);
bool isMethodAllowd(const LocationConfig& loc, const std::string& method);
std::string resolvePath(const ServerConfig& server, const LocationConfig* loc, const std::string& uri);