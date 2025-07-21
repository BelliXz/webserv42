
#ifndef Server_HPP
#define Server_HPP

#include "ConfigParser.hpp"
#include <iostream>

class Server
{
	private:
		std::vector<ServerConfig>	serverConfigs;	
		std::string					configFile;		

		size_t	parseConfig(std::string config_file);		
	public:
		Server(std::string config_file);

		// add 2 ft
		const std::vector<ServerConfig>& getServerConfigs() const ;
		static void printServerConfigs(const std::vector<ServerConfig>& servers) ;
};



#endif

