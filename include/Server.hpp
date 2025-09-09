
#ifndef Server_HPP
#define Server_HPP

#include "ConnectionManager.hpp"
#include "ServerConfig.hpp"

#define 	SERV_MAX_CONNS  	100
#define		SERV_MAX_EVENTS 	1042
#define		SERV_WAIT_TIMEOUT	1000

class Server
{
	private:
		std::vector<int>			serverSockets;		
		std::vector<ServerConfig>	serverconfigs;	
		std::string					configFile;
		ConnectionManager			cm;

		bool						isServerSocket(int fd);
		
		
	public:
		Server();
		~Server();
		Server(std::string config_file);

		const std::vector<ServerConfig>& getServerConfigs() const ;
		bool 						start(ConnectionManager& cm);
		int 						run();


		// Helper functions for parseAllConfigs
		// static std::string 				trim(const std::string& s) 
		// static std::vector<std::string> 	split(const std::string& line, char delim = ' ') 

		std::vector<ServerConfig> 	parseAllConfigs(const std::string& configFilePath);
		bool 						checkErrorParseAllConfigs(const std::vector<ServerConfig>& servers);
		
		static void 				printAllServerConfig(const std::vector<ServerConfig>& servers) ;
		static void 				printSelectedServer(const ServerConfig* s);
		static void 				printAllRoutes(const std::map<std::string, RouteConfig>& routes);
		static void					printSelecteRoutes(const RouteConfig &route);

};



#endif

