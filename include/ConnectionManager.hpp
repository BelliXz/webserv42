#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

# include 	"Connection.hpp"


class ConnectionManager 
{
	private:

		std::map<int, Connection> 	connections; 
		std::map<int, ServerConfig> servers;
		std::vector<ServerConfig>	rawServers;


	public:
		static int					epollFd; 

		ConnectionManager();		
		~ConnectionManager();


		int							addRawServer(ServerConfig server);
		std::vector<ServerConfig>   getRawServers();

		static void					setEpollFd(int epollscoket);
		static  int					getEpollFd();

		// int						addServer(int socket, ServerConfig server);
		ServerConfig				*getServer(int socket);
		std::map<int, ServerConfig> getServers(); 


		Connection 					*findConnection(int socket);
		bool						closeConnection(int socket);
		int							openConnection(int socket, ServerConfig config);





		//size_t					purgeExpiredConnections();
		//bool 						handleRequestException(RequestException &reqException,Connection &conn);
		//void						debug();
		bool						read(int clientSocket);
		bool						write(int clientSocket);

};

#endif
