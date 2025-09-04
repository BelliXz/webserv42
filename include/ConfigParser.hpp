

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "ServerConfig.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>        



class ConfigParser
{
	public:
		static std::vector<ServerConfig> parseAllConfigs		  (const std::string& configFilePath);
		static bool 					 checkErrorParseAllConfigs(const std::vector<ServerConfig>& servers);
		static void 					 printAllServerConfig		  (const std::vector<ServerConfig>& servers) ;


		static void 					 printSelectedServer(const ServerConfig* s);


		
};


#endif

