#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <string>
#include <map>
#include "httprequest.hpp"

class CgiHandler {
public:
    CgiHandler(const HttpRequest& request, const std::string& scriptPath);

    std::string execute();

private:
    const HttpRequest& request;
    std::string scriptPath;
    std::map<std::string, std::string> env;

    void freeEnv(char** envp) const;
    void buildEnv();
    char** envToCharArray() const;
};

#endif