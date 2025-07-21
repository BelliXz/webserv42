
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfigParser.hpp"
#include <iostream>

class Webserv
{
	private:
		std::vector<ServerConfig>	serverConfigs;	
		std::string					configFile;		

		size_t	parseConfig(std::string config_file);		
	public:
		Webserv(std::string config_file);

		// add 2 ft
		const std::vector<ServerConfig>& getServerConfigs() const ;
		static void printServerConfigs(const std::vector<ServerConfig>& servers) ;
};



#endif

