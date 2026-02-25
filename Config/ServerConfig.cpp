# include "ServerConfig.hpp"

ServerConfig& findServer(std::vector<ServerConfig>& servers, const std::string& host, int port) {
    ServerConfig* fallback = NULL;
    for(size_t i = 0; i < servers.size(); i++) {
        if (servers[i].listen == port) {
            if (!fallback)
                fallback = &servers[i];
            if (servers[i].host == host)
                return servers[i];
        }
    }
    if (fallback)
        return *fallback;
    else
        return servers[0];
}
