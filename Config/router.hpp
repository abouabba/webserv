# ifndef ROUTER_HPP
# define ROUTER_HPP
# include <string>
# include "httprequest.hpp"
# include "ServerConfig.hpp"
enum RouteType {
    ROUTE_STATIC_FILE,
    ROUTE_CGI,
    ROUTE_ERROR
};

struct RouteResult {
    public:
        RouteType type;
        std::string path;
        int statusCode; // 200, 403, 404, 405...

        RouteResult(RouteType type, const std::string& path, int status);
};

RouteResult routeRequest(const HttpRequest& request, std::vector<ServerConfig>& servers, int port);
# endif