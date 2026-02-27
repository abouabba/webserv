#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <string>
#include <vector>

// ✅ forward declaration
class ServerConfig;

struct LocationConfig {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    bool autoindex;
};

// functions declarations
LocationConfig* findLocation(ServerConfig& server, const std::string& uri);
std::string resolvePath(const ServerConfig& server,
                        const LocationConfig* loc,
                        const std::string& uri);

bool isMethodAllowd(const LocationConfig& loc, const std::string& method);

#endif