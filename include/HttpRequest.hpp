
#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <cstdlib>  
#include <iomanip>

# define RED 	"\033[0;31m"
# define GREEN 	"\033[0;32m" 
# define RESET  "\033[0m"

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

        int         parseHttpRequest(char* buffer, HttpRequest& req);
        static void printHttpRequest(const HttpRequest& req);
};


#endif
