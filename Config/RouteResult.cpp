# include "RouteResult.hpp"
# include "httprequest.hpp"
# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
#include <sys/stat.h>

bool fileExists(const std::string& path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

bool isCGI(const std::string& path) {
    size_t dot = path.find('.');
    if (dot == std::string::npos)
        return false;

    std::string ext = path.substr(dot);
    return (ext == ".php" || ext == ".py" || ext == ".cgi");
}

RouteResult routeRequest(const HttpRequest& request, std::vector<ServerConfig>& servers, int port) {

    ServerConfig& server = findServer(servers, request.headers.at("Host"), port);

    LocationConfig* location = findLocation(server, request.uri);

    if (location && !isMethodAllowd(*location, request.method)) {
        return {ROUTE_ERROR, "", 405};
    }

    std::string fullpath = resolvePath(server, location, request.uri);

    if (isCGI(fullpath)) {
        return {ROUTE_CGI, fullpath, 200};
    }

    if (fileExists(fullpath)) {
        return {ROUTE_STATIC_FILE, fullpath, 200};
    }

    return {ROUTE_ERROR, fullpath, 404};

}