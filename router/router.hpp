# ifndef ROUTER_HPP
# define ROUTER_HPP
# include <string>

enum RouteType {
    ROUTE_STATIC_FILE,
    ROUTE_CGI,
    ROUTE_ERROR
};

struct RouteResult {
    RouteType type;
    std::string path;
    int statusCode; // 200, 403, 404, 405...
};

# endif