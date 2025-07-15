#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <map>

struct RouteConfig 
{
    std::string                         path;
    std::string                         root;
    std::string                         index;
    std::vector<std::string>            allow_methods;
    std::map<std::string,std::string>   CGIs;
    std::string                         uploadStore;
    size_t                              clientMaxBodySize;
    std::string                         redirectPath;
    int                                 redirectStatus;
    bool                                autoIndex;

    RouteConfig() : autoIndex(false), clientMaxBodySize(1024 * 1024 * 8) {}
};

struct ServerConfig 
{
    std::string host;
    int port;
    std::vector<std::string> serverNames;
    std::vector<RouteConfig> routes;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;

    ServerConfig() : host("0.0.0.0"), port(0), clientMaxBodySize(1024 * 1024 * 8) {}
};

#endif 