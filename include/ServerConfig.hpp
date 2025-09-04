

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP


#include "RouteConfig.hpp"

class ServerConfig 
{
	private:
		int 								port;
		int 								portFd;
		std::string 						serverName;
		std::string 						host;
		std::string 						root;
		std::string 						index;
		size_t 								clientMaxBodySize;
		std::map<int, std::string> 			errorPages;
		std::map<std::string, RouteConfig> 	routes;		
	
	public:
		ServerConfig();

		void setPortFd(int fd) { portFd = fd; }	
		int getPortFd() const { return portFd; } 

		int 		getPort() const;
		std::string getServerName() const;
		std::string getHost() const;
		std::string getRoot() const;
		std::string getIndex() const;
		size_t 		getClientMaxBodySize() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::map<std::string, RouteConfig>& getRoutes() const;


		void setPort(int p);
		void setServerName(const std::string& name);
		void setHost(const std::string& h);
		void setRoot(const std::string& r);
		void setIndex(const std::string& i);
		void setClientMaxBodySize(size_t size) ;

		void setErrorPages(const std::map<int, std::string>& errorPages);	
		void setRoutes(const std::map<std::string, RouteConfig>& routes);	
	
		void addRoute(const RouteConfig& routeConfig);						
		void addRoute(const std::string& path, const RouteConfig& route);
		void addErrorPage(int code, const std::string& path);

	};

#endif
