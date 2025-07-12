

#include "Core.hpp"






// ---------- Struct Definitions ----------
struct RouteConfig 
{
    std::string                         path;
    std::string                         root;
    std::string                         index;
    std::vector<std::string>            methods;
    std::map<std::string, std::string>  cgiExtensions;
    std::string                         uploadStore;  
    bool                                autoIndex;          // = false
    size_t                              clientMaxBodySize;  // = 0
    int                                 redirectStatus;     // = 0
    std::string                         redirectPath;

    RouteConfig(): autoIndex(false), clientMaxBodySize(0), redirectStatus(0) 
    {
    }
};



struct ServerConfig 
{
    std::string                         host;
    int                                 port;
    std::vector<std::string>            serverNames;
    std::vector<RouteConfig>            routes;
    std::map<int, std::string>          errorPages;
    size_t                              clientMaxBodySize;  // = 0

    ServerConfig(): clientMaxBodySize(0) 
    {
    }
};



