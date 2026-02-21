# include "ConfigParser.hpp"

int main(int ac, char **av) {
    ConfigParser config;
    if (ac != 2)
        throw std::runtime_error("invalid argument");
    config.parse(av[1]);
    return 0;
}