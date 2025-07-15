
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





class HttpRequest 
{
    public:

        std::string             filename;                   
        std::string             method;                   
        std::string             path;                      
        std::string             query;                    
        std::string             version;                   
        std::string             afterVersion;
        std::string             body;                      
        std::vector<char>       rawBody;                   
        std::string             contenttype;              
        std::string             cookie;                   
        std::map<std::string, std::string> headers;       
        bool                    complete;      
        size_t                  contentLength;  


        HttpRequest() 
            : complete(false), contentLength(0) 
        {}
};


