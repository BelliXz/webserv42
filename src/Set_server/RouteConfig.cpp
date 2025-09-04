
#include "../include/RouteConfig.hpp"

RouteConfig::RouteConfig() : autoindex(false), clientMaxBodySize(0), returnStatus(0) {}


const std::string& RouteConfig::getPath() const 						{ return path; } 
std::string RouteConfig::		getRoot() const 						{ return root; }
std::string RouteConfig::		getIndex() const 						{ return index; }
const std::vector<std::string>& RouteConfig::getMethods() const 		{ return allow_methods; }


const std::map<std::string, std::string>& RouteConfig::getCGIs() const 	{ return CGIs; }



std::string RouteConfig::getCGI(std::string ext)
{
    if (!ext.empty() && ext[0] != '.')
        ext = "." + ext;

    std::map<std::string, std::string>::const_iterator it = CGIs.find(ext);
    if (it != CGIs.end())
    {
        std::string cmd = it->second;

        if (!cmd.empty() && cmd[cmd.size() - 1] == ';')
            cmd.resize(cmd.size() - 1);
        return cmd;
    }
    return "";
}


bool RouteConfig::				getAutoindex() const 					{ return autoindex; }
size_t RouteConfig::			getClientMaxBodySize() const 			{ return clientMaxBodySize; }
std::string RouteConfig::		getUploadStore() const 					{ return uploadStore; }
int RouteConfig::				getReturnStatus() const 				{ return returnStatus; }
std::string RouteConfig::		getReturnValue() const	 				{ return returnValue; }


void RouteConfig::setPath	(const std::string& p) 							  { path = p; }
void RouteConfig::setRoot	(const std::string& r) 							  { root = r; }
void RouteConfig::setIndex	(const std::string& i) 							  { index = i; }
void RouteConfig::setMethods(const std::vector<std::string>& methods) 		  { allow_methods = methods;}
void RouteConfig::setCGIs(const std::string& ext, const std::string& path) 	  { CGIs[ext] = path; }
void RouteConfig::setAutoindex	(bool val) 									  { autoindex = val; }
void RouteConfig::setClientMaxBodySize(size_t size) 						  { clientMaxBodySize = size; }
void RouteConfig::setReturnStatus(int status) 								  { returnStatus = status; }
void RouteConfig::setReturnValue(const std::string &value) 					  { returnValue = value; }
void RouteConfig::setUploadStore(const std::string& path) 					  { uploadStore = path; }


void RouteConfig::addMethod	(const std::string& m) 							  { allow_methods.push_back(m); }
void RouteConfig::addCGI	(const std::string& ext, const std::string& path) { CGIs[ext] = path; }
void RouteConfig::setReturn		(int status, const std::string& target) 
{
	returnStatus = status;
	returnValue = target;
}


void RouteConfig::printRouteConfig(const RouteConfig &route) {
    std::cout << "======= RouteConfig::printRouteConfig 81 ========\n";
    std::cout << SERVER_COLOR << "---- RouteConfig::printRouteConfig ----" << RESET << std::endl;
    std::cout << SERVER_COLOR << "  Path: " << route.getPath() << SERVER_COLOR << RESET << "\n";
		std::cout << "    Root: " << route.getRoot() << "\n";
		std::cout << "    Index: " << route.getIndex() << "\n";
		std::cout << "    Autoindex: " << (route.getAutoindex() ? "on" : "off") << "\n";
		std::cout << "    Client Max Body Size: " << route.getClientMaxBodySize() << "\n";
		std::cout << "    Upload Store: " << route.getUploadStore() << "\n";
		// Allowed Methods
		std::cout << "    Allow Methods: ";
		const std::vector<std::string>& methods = route.getMethods();
		for (size_t j = 0; j < methods.size(); ++j) 
		{
			std::cout << methods[j];
			if (j + 1 < methods.size()) std::cout << ", ";
		}
		std::cout << "\n";
		// CGI Map
		std::cout << "    CGI Passes:\n";
		const std::map<std::string, std::string>& cgis = route.getCGIs();
		for (std::map<std::string, std::string>::const_iterator cit = cgis.begin(); cit != cgis.end(); ++cit) 
		{
			std::cout << "      " << cit->first << " => " << cit->second << "\n";
		}
		// Return Directive
		if (route.getReturnStatus() != 0) 
		{
			std::cout << "    Return: " << route.getReturnStatus() << " => " << route.getReturnValue() << "\n";
		}
    std::cout << "---------------------" << std::endl;
}
