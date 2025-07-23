
#ifndef Server_HPP
#define Server_HPP

#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include 	<iostream>
#include 	<vector>
#include 	<algorithm>
#include 	<set>
#include 	<sys/socket.h>
#include	<netinet/in.h>
#include	<sys/epoll.h>
#include 	<fcntl.h>
#include 	<string>
#include 	<signal.h>
#include 	<cstring>



#define 	SERV_MAX_CONNS 100

class ServerConfig; 
class ConnectionManager;

class Server
{
	private:
		std::vector<ServerConfig>	serverConfigs;	
		std::string					configFile;
		std::vector<int>			serverSockets;		

		size_t	parseConfig(std::string config_file);
		
		//bool						start(ConnectionController &cc);
		void 						start(const std::vector<ServerConfig>& configs);
		//bool						findServerFromRequest(HttpRequest &req, ServerConfig &sc);
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

