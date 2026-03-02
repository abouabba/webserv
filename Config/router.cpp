# include "router.hpp"
# include "httprequest.hpp"
# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
// # include "RouteResult.hpp"
#include <sys/stat.h>

RouteResult::RouteResult(RouteType type, const std::string& path, int status) 
    : type(type), path(path), statusCode(status) {}

ServerConfig& findServer(std::vector<ServerConfig>& servers, const std::string& host, int port) {
    ServerConfig* fallback = NULL;
    for(size_t i = 0; i < servers.size(); i++) {
        if (servers[i].listen == port) {
            if (!fallback)
                fallback = &servers[i];
            if (servers[i].host == host)
                return servers[i];
        }
    }
    if (fallback)
        return *fallback;
    else
        return servers[0];
}

LocationConfig* findLocation(ServerConfig& server, const std::string& uri) {
    LocationConfig* best = NULL;
    size_t bestLen = 0;

    for(size_t i = 0; i < server.locations.size(); i++) {
            const std::string& path = server.locations[i].path;
            if (uri.compare(0, path.size(), path) == 0) {
                if (path.size() > bestLen) {
                    best = &server.locations[i];
                    bestLen = path.size();
                }
            }
    }
    return best;
}

bool isMethodAllowd(const LocationConfig& loc, const std::string& method) {
    if(loc.methods.empty())
        return true;
    for(size_t i = 0; i < loc.methods.size(); i++) {
        if (loc.methods[i] == method)
            return true;
    }
    return false;
}

std::string resolvePath(const ServerConfig& server, const LocationConfig* location, const std::string& uri)
{
    if (location && !location->root.empty())
    {
        std::string relative = uri.substr(location->path.length());
        if (!relative.empty() && relative[0] == '/')
            relative.erase(0, 1);
        return location->root + "/" + relative;
    }
    return server.root + uri;
}

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
        return RouteResult(ROUTE_ERROR, "", 405);
    }

    std::string fullpath = resolvePath(server, location, request.uri);

    if (isCGI(fullpath)) {
        return RouteResult(ROUTE_CGI, fullpath, 200);
    }

    if (fileExists(fullpath)) {
        return RouteResult(ROUTE_STATIC_FILE, fullpath, 200);
    }



    return RouteResult(ROUTE_ERROR, fullpath, 404);

}
