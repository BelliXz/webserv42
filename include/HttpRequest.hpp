
#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <cstdlib>  
#include <iomanip>

#include "RouteConfig.hpp"


class HttpRequest 
{
    public:
        std::string                         request;       
        std::string                         method;             
        std::string 			            rawPath;        
        std::string                         path;                      
        std::string                         filename;                   
        std::string				            rawQueryString;
        std::string                         query;                    
        std::map<std::string, std::string> 	queryStrings;   
        std::string                         version;                   
        std::string                         afterVersion;   
        std::map<std::string, std::string>  headers;        
        std::string                         host;           
        int                                 port;           
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


