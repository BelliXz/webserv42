
#ifndef CONNECTIONCONROUTECONFIG_HPP
#define CONNECTIONCONROUTECONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <errno.h>
#include <sstream>
#include <sys/stat.h>
#include <fstream>
#include "RequestException.hpp"


# define FD_COLOR 		"\033[0;35m"
# define REQUEST_COLOR 	"\033[0;34m"
# define SERVER_COLOR	"\033[0;33m"
# define MONITOR_COLOR	"\033[0;32m"
# define ERROR_COLOR	"\033[0;31m"
# define RESPONSE_COLOR "\033[0;35m"

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


class RouteConfig 
{
	private:
		std::string 						path;
		std::string 						root;
		std::string 						index;
		std::vector<std::string> 			allow_methods;
		std::map<std::string, std::string> 	CGIs;
		bool 								autoindex;
		size_t 								clientMaxBodySize;
		int 								returnStatus;
		std::string 						returnValue;
		std::string 						uploadStore;
	
	public:
		RouteConfig();


		const std::string& 	getPath() const ; 		
		std::string 		getRoot() const ;
		std::string 		getIndex() const ;
		const std::vector<std::string>& getMethods() const ;
		const std::map<std::string, std::string>& getCGIs() const;	//CGIs
		std::string 		getCGI(std::string);					//CGI
		bool 				getAutoindex() const ;
		size_t 				getClientMaxBodySize() const ;
		std::string 		getUploadStore() const;
		int 				getReturnStatus() const ;
		std::string 		getReturnValue() const ;

		
		void setPath(const std::string& p);
		void setRoot(const std::string& r);
		void setIndex(const std::string& i);
		void setMethods(const std::vector<std::string>& methods);
		void setCGIs(const std::string& ext, const std::string& path);
		void setAutoindex(bool val);
		void setClientMaxBodySize(size_t size);
		void setReturnStatus(int status);	
		void setReturnValue(const std::string &value);	
		void setUploadStore(const std::string& path);
	
		//add 3 ft
		void addMethod(const std::string& m);
		void addCGI(const std::string& ext, const std::string& path);
		void setReturn(int status, const std::string& target);
	
		static void printRouteConfig(const RouteConfig &route) ;

};

#endif
