// # include "ConfigParser.hpp"
// # include "httprequest.hpp"
// # include "CgiHandler.hpp"

// int main(int ac, char **av) {
//     ConfigParser config;
//     if (ac != 2)
//         throw std::runtime_error("invalid argument");
//     try {

//         config.parse(av[1]);
//     }
//     catch (const std::exception& e) {
//         std::cout << e.what() << std::endl;
//     }
//     return 0;
// }

#include "ConfigParser.hpp"
#include "router.hpp"
#include "CgiHandler.hpp"
#include <iostream>

int main(int ac, char**av)
{
    (void)ac;
    try
    {
        // 1️⃣ Parse config
        // ConfigParser parser("config.conf");
        // std::vector<ServerConfig> servers = parser.parse();
        std::vector<ServerConfig> servers =
    ConfigParser::parse(av[1]);

        std::cout << "Config parsed: " << servers.size() << " server(s)\n";

        // 2️⃣ Fake HTTP request
        HttpRequest req;
        req.method = "GET";
        req.uri = "/cgi-bin/test.py";
        req.headers["Host"] = "localhost";

        // 3️⃣ Route request
        RouteResult result = routeRequest(req, servers, 8080);

        std::cout << "Matched path: " << result.path << "\n";
        std::cout << "Status code: " << result.statusCode << "\n";
        
        // Execute CGI
        if (result.type == ROUTE_CGI)
        {
            CgiHandler cgi(req, result.path);
            std::string output = cgi.execute();
        
            std::cout << "\n===== CGI OUTPUT =====\n";
            std::cout << output << std::endl;
        }
        else
        {
            std::cout << "Not a CGI request\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}