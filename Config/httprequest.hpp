# ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <map>

struct HttpRequest {
    std::string method;   // "GET", "POST", "DELETE"
    std::string uri;      // "/upload/file.txt"
    std::string version;  // "HTTP/1.0"
    std::map<std::string, std::string> headers;
    std::string body;
};

# endif
