#include "ConfigParser.hpp"
#include <fstream>
#include <stdexcept>


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
    // ServerConfig currentServer;
    // LocationConfig currentLocation;

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
            // currentServer = ServerConfig();
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
            // currentLocation = LocationConfig();
            // currentLocation.path = path;
            state = INSIDE_LOCATION;
            continue;
        }


        if (line == "}") {
            if (state == INSIDE_LOCATION) {
                std::cout << "EXIT LOCATION" << std::endl;
                // currentServer.locations.push_back(currentLocation);
                state = INSIDE_SERVER;
            }
            else if (state == INSIDE_SERVER) {
                std::cout << "EXIT SERVER" << std::endl;
                // servers.push_back(currentServer);
                state = OUTSIDE;
            }
            else {
                throw std::runtime_error("Unexpected }");
            }
            continue;
        }

        /* ---------- directives (ignored for now) ---------- */
        if (state == INSIDE_SERVER || state == INSIDE_LOCATION) {
            // directives like listen, root, methods...
            // will be parsed later
            continue;
        }

        throw std::runtime_error("Invalid line outside blocks: " + line);
    }

    if (state != OUTSIDE)
        throw std::runtime_error("Unclosed block at end of file");

    return servers;

}
