#pragma once
#include <vector>
#include <string>
#include "ServerConfig.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

enum ParseState {
    OUTSIDE,
    INSIDE_SERVER,
    INSIDE_LOCATION
};

class ConfigParser {
public:
    static std::vector<ServerConfig> parse(const std::string& filename);
};