
#ifndef Server_HPP
#define Server_HPP

#include "ConfigParser.hpp"
#include "ConnectionManager.hpp"

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
		// ConnectionManager			connectionmanager;
		ConnectionManager			cm;

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

		const std::vector<ServerConfig>& getServerConfigs() const ;
		static void printServerConfigs(const std::vector<ServerConfig>& servers) ;





		// static void printRoutes(const std::map<std::string, RouteConfig>& routes);
		// RouteConfig selectServerAndRoute(HttpRequest& request, ServerConfig& server, std::string& fullPath);
};



#endif

