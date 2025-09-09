
#include "../include/ServerConfig.hpp"

ServerConfig::ServerConfig() : port(0), clientMaxBodySize(0) {}
// ServerConfig::ServerConfig() : port(0), host("0.0.0.0"), clientMaxBodySize(1024 * 1024 * 8) {}	// check

void ServerConfig::			setPortFd(int fd) 								{ portFd = fd; }
int ServerConfig::			getPortFd() const 								{ return portFd; }


int ServerConfig::			getPort() const 								{ return port; }
std::string ServerConfig::	getServerName() const 							{ return serverName; }
std::string ServerConfig::	getHost() const 								{ return host; }
std::string ServerConfig::	getRoot() const 								{ return root; }
std::string ServerConfig::	getIndex() const 								{ return index; }
size_t ServerConfig::		getClientMaxBodySize() const 					{ return clientMaxBodySize; }
const std::map<int, std::string>& ServerConfig::getErrorPages() const 		{ return errorPages; }
const std::map<std::string, RouteConfig>& ServerConfig::getRoutes() const	{ return routes; }



void ServerConfig::setPort		(int p) 											{ port = p; }
void ServerConfig::setServerName(const std::string& name) 							{ serverName = name; }
void ServerConfig::setHost		(const std::string& h) 								{ host = h; }
void ServerConfig::setRoot		(const std::string& r) 								{ root = r; }
void ServerConfig::setIndex		(const std::string& i) 								{ index = i; }
void ServerConfig::setClientMaxBodySize(size_t size) 								{ clientMaxBodySize = size; }



void ServerConfig::setErrorPages(const std::map<int, std::string>& errorPages) 		{ this->errorPages = errorPages; }
void ServerConfig::setRoutes(const std::map<std::string, RouteConfig>& routes) 		{ this->routes = routes; }
void ServerConfig::addRoute(const RouteConfig& routeConfig)							{routes[routeConfig.getPath()] = routeConfig;}


void ServerConfig::addRoute		(const std::string& path, const RouteConfig& route) { routes[path] = route; }
void ServerConfig::addErrorPage	(int code, const std::string& path) 				{ errorPages[code] = path; }

