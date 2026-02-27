# include "LocationConfig.hpp"
// # include "ServerConfig.hpp"

// LocationConfig* findLocation(ServerConfig& server, const std::string& uri) {
//     LocationConfig* best = NULL;
//     size_t bestLen = 0;

//     for(size_t i = 0; i < server.locations.size(); i++) {
//             const std::string& path = server.locations[i].path;
//             if (uri.compare(0, path.size(), path) == 0) {
//                 if (path.size() > bestLen) {
//                     best = &server.locations[i];
//                     bestLen = path.size();
//                 }
//             }
//     }
//     return best;
// }

// bool isMethodAllowd(const LocationConfig& loc, const std::string& method) {
//     if(loc.methods.empty())
//         return true;
//     for(size_t i = 0; i < loc.methods.size(); i++) {
//         if (loc.methods[i] == method)
//             return true;
//     }
//     return false;
// }

// std::string resolvePath(const ServerConfig& server, const LocationConfig* loc, const std::string& uri) {
//         std::string base = NULL;
//         if(!loc->root.empty())
//             base = loc->root;
//         else
//             base = server.root;
    
//         if (loc)
//             return base + uri.substr(loc->path.size());
//         return base + uri;
// }

