# include "ConfigParser.hpp"

int main(int ac, char **av) {
    ConfigParser config;
    if (ac != 2)
        throw std::runtime_error("invalid argument");
    try {

        config.parse(av[1]);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}