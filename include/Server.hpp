
#ifndef Server_HPP
#define Server_HPP

#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include "ConnectionManager.hpp"
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



#define 	SERV_MAX_CONNS  	100
#define		SERV_MAX_EVENTS 	1042
#define		SERV_WAIT_TIMEOUT	1000

class ServerConfig; 
class ConnectionManager;

class Server
{
	private:
		std::vector<int>			serverSockets;		
		std::vector<ServerConfig>	serverconfigs;	
		std::string					configFile;
		ConnectionManager			Connectionmanager;

		size_t	parseConfig(std::string config_file);
		
		//bool						start(ConnectionController &cc);
		//bool						findServerFromRequest(HttpRequest &req, ServerConfig &sc);
		bool						isServerSocket(int fd);
		
		
		public:
		Server();
		~Server();
		Server(std::string config_file);

		bool 				start(ConnectionManager& cm);
		
		int run();

		std::set<int> 				getListeningPorts();
		//ConnectionController 		&getConnectionController();

		std::vector<ServerConfig>& GetServerConfigs();

		// add 2 ft
		const std::vector<ServerConfig>& getServerConfigs() const ;
		static void printServerConfigs(const std::vector<ServerConfig>& servers) ;
};



#endif

