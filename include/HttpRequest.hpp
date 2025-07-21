
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
        std::string                         request;        // kit add original request
        std::string                         method;             
        std::string 			            rawPath;        // Get add
        std::string                         path;                      
        std::string                         filename;                   
        std::string				            rawQueryString; // Get add
        std::string                         query;                    
        std::map<std::string, std::string> 	queryStrings;   // Get add
        std::string                         version;                   
        std::string                         afterVersion;   // kit add check error
        std::map<std::string, std::string>  headers;       
        size_t                              contentLength;  
        std::string                         contenttype;              
        std::string                         cookie;                   
        std::vector<char>                   rawBody;                   
        std::string                         body;        
        bool                                complete;      

        HttpRequest();


        int         parseHttpRequest(char* buffer, HttpRequest& req);
        static void printHttpRequest(const HttpRequest& req);
};


#endif


