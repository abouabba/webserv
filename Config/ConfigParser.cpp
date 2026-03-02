#include "ConfigParser.hpp"
#include <fstream>
#include <stdexcept>


bool isValidMethod(const std::string &t) {
    return (t == "GET" || t == "POST" || t == "DELETE");
}

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(s);
    while(iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t last = s.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    return s.substr(start, last - start + 1);
}

std::vector<ServerConfig> ConfigParser::parse(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Cannot open config file");

    std::vector<ServerConfig> servers;
    ServerConfig currentServer;
    LocationConfig currentLocation;

    std::string line;
    ParseState state = OUTSIDE;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || line[0] == '#')
            continue;


        if (line == "server {") {
            if (state != OUTSIDE)
                throw std::runtime_error("server block inside another block");

            std::cout << "ENTER SERVER" << std::endl;
            currentServer = ServerConfig();
            state = INSIDE_SERVER;
            continue;
        }

        if (state == INSIDE_SERVER && line.find("location") == 0) {
            size_t bracePos = line.find("{");
            if (bracePos == std::string::npos)
                throw std::runtime_error("location without {");

            std::string path = trim(line.substr(8, bracePos - 8));
            if (path.empty())
                throw std::runtime_error("location without path");

            std::cout << "ENTER LOCATION " << path << std::endl;
            currentLocation = LocationConfig();
            currentLocation.path = path;
            state = INSIDE_LOCATION;
            continue;
        }


        if (line == "}") {
            if (state == INSIDE_LOCATION) {
                std::cout << "EXIT LOCATION" << std::endl;
                currentServer.locations.push_back(currentLocation);
                state = INSIDE_SERVER;
            }
            else if (state == INSIDE_SERVER) {
                std::cout << "EXIT SERVER" << std::endl;
                if (currentServer.listen == -1)
                    throw std::runtime_error("server missing listen");
                if (currentServer.root.empty())
                    throw std::runtime_error("server missing root");

                servers.push_back(currentServer);
                state = OUTSIDE;
            }
            else {
                throw std::runtime_error("Unexpected }");
            }
            continue;
        }

        if (state == INSIDE_SERVER) {
            if (line[line.size() - 1] != ';')
                throw std::runtime_error("Missing ;");
            line.erase(line.size() - 1);
            if (line[line.size() - 1] == ';')
                throw std::runtime_error("extra simicolon ;");
            std::vector<std::string> tokens = split(line);
            if (tokens.empty())
                continue;

            if (tokens[0] == "listen") {
                if (tokens.size() != 2)
                    throw std::runtime_error("Invalid listen directives");
                currentServer.listen = std::atoi(tokens[1].c_str());
            }
            else if (tokens[0] == "host") {
                if (tokens.size() != 2)
                    throw std::runtime_error("Invalid host directives");
                currentServer.host = tokens[1];
            }
            else if (tokens[0] == "root") {
                if (tokens.size() != 2)
                    throw std::runtime_error("Invalid root directives");
                currentServer.root = tokens[1];
            }
            continue;
        }
        if (state == INSIDE_LOCATION) {
            if (line[line.size() - 1] != ';')
                throw std::runtime_error("Missing ;");
            line.erase(line.size() - 1);
            if (line[line.size() - 1] == ';')
                throw std::runtime_error("extra simicolon ;");
            std::vector<std::string> t = split(line);
            if (t[0] == "methods") {
                if (t.size() < 2)
                    throw std::runtime_error("methods empty");
                for (size_t i = 1; i < t.size(); i++) {
                    if (!isValidMethod(t[i]))
                        throw std::runtime_error("Invalid http method");
                    currentLocation.methods.push_back(t[i]);
                }
            }
            else if (t[0] == "root") {
                if (t.size() != 2)
                throw std::runtime_error("Invalid root in location");
            currentLocation.root = t[1];
            }
            else if (t[0] == "autoindex") {
                if (t.size() != 2)
                    throw std::runtime_error("Invalid autoindex");
                if (t[1] == "on")
                    currentLocation.autoindex = true;
                else if (t[1] == "off")
                    currentLocation.autoindex = false;
                else
                    throw std::runtime_error("autoindex must be on/off");
            }
            else
                throw std::runtime_error("Unknown directive in location");
            
            continue;
        }
        throw std::runtime_error("Invalid line : " + line);
    }

    if (state != OUTSIDE)
        throw std::runtime_error("Unclosed block");

    for(size_t i = 0; i < servers.size(); i++) {

        std::cout << "listen :" << servers[i].listen << std::endl;
        std::cout << "host :" << servers[i].host << std::endl;
        std::cout << "root :" << servers[i].root << std::endl;
    }
    return servers;

}
