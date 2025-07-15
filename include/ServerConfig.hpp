
# define BOLD 	"\033[1m"
# define RED 	"\033[0;31m"
# define GREEN 	"\033[0;32m"
# define BLUE	"\033[0;34m"
# define CYAN 	"\033[0;36m"
# define GRAY 	"\033[0;37m"
# define LRED 	"\033[0;91m"
# define YELLOW	"\033[0;33m"
# define MAGENTA "\033[0;35m" 
# define RESET  "\033[0m"
# define RGB 	"\033[1m\033[38;2;255;182;193m"


#include <sstream>
#include <iostream>
#include <cstring>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdlib>  
#include <iomanip>

#include <fstream>
#include <cstdio>      
#include <sys/socket.h> 
#include <cctype>




struct RouteConfig 
{
    std::string                         path;
    std::string                         root;
    std::string                         index;
    std::vector<std::string>            methods;
    std::map<std::string, std::string>  cgiExtensions;
    std::string                         uploadStore;  
    bool                                autoIndex;          
    size_t                              clientMaxBodySize;  
    int                                 redirectStatus;     
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
    size_t                              clientMaxBodySize;

    ServerConfig(): clientMaxBodySize(0) 
    {
    }
};


std::map<int, std::vector<ServerConfig> > ft_parseConfigFile(const std::string& filename) ;
