#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

# include 	<map>
# include	<unistd.h>
# include	<string.h>
# include	<cstring>
# include 	"Connection.hpp"
# include 	"ServerConfig.hpp"

//#define CONNECTION_TIMEOUT 60

class Connection;
class ConnectionManager 
{
	private:

		std::map<int, Connection> 	connections; 
		std::map<int, ServerConfig> servers;
		std::vector<ServerConfig>	rawServers;

		// ConnectionManager(ConnectionManager const &other);
		// ConnectionManager &operator=(ConnectionManager const &other);

	public:
		ConnectionManager();		
		~ConnectionManager();
		void setConfigs(const std::vector<ServerConfig>& configs);

		static int					epollFd; 

		// Connection 					*findConnection(int socket);
		bool						closeConnection(int socket);
		// int							openConnection(int socket, ServerConfig config);

		// bool						handleRead(int clientSocket);
		// bool						handleWrite(int clientSocket);

		// int							addServer(int socket, ServerConfig server);
		ServerConfig				*getServer(int socket);
		std::map<int, ServerConfig> getServers(); 



		// size_t						purgeExpiredConnections();
		//bool 						handleRequestException(RequestException &reqException,Connection &conn);
		//void						debug();

		int							addRawServer(ServerConfig server);
		std::vector<ServerConfig>   getRawServers();

		static void					setEpollFd(int epollscoket);
		static  int					getEpollFd();

};

#endif
