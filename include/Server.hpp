
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
		
		//bool						start(ConnectionController &cc);
		void 						start(const std::vector<ServerConfig>& configs);
		bool						findServerFromRequest(HttpRequest &req, ServerConfig &sc);
		bool						isServerSocket(int fd);

		
	public:
		Server(std::string config_file);
		~Server();

		int run(void);

		std::set<int> 				getListeningPorts();
		//ConnectionController 		&getConnectionController();

		// add 2 ft
		const std::vector<ServerConfig>& getServerConfigs() const ;
		static void printServerConfigs(const std::vector<ServerConfig>& servers) ;
};



#endif

