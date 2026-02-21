#pragma once
#include <vector>
#include <string>
#include "ServerConfig.hpp"
#include <iostream>

enum ParseState {
    OUTSIDE,
    INSIDE_SERVER,
    INSIDE_LOCATION
};

class ConfigParser {
public:
    std::vector<ServerConfig> parse(const std::string& filename);
};
