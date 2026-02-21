#pragma once
#include <string>
#include <vector>

struct LocationConfig {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    bool autoindex;

    LocationConfig() : autoindex(false) {}
};
