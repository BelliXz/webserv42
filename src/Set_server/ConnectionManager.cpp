
#include "ConnectionManager.hpp"

int ConnectionManager::epollFd = 0; 

ConnectionManager::ConnectionManager()
{
	std::cout<< "ConnectionManage  has been called"<<std::endl;
}

ConnectionManager::~ConnectionManager()
{
    std::cout<< "ConnectionManage destructor has been called"<<std::endl;
    for(std::map<int, Connection>::iterator it = connections.begin(); it!= connections.end(); ++it )
		closeConnection(it->first);
}



std::map<int, ServerConfig> ConnectionManager::getServers()
{
	return servers;
}

int		ConnectionManager::openConnection(int clientSocket, ServerConfig serverConfig)
{

	int flags = fcntl(clientSocket, F_GETFL, 0);
	if (flags < 0 || fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) < 0) 
	{
		std::cerr << "CAN NOT ACCEPTED SOCKET TO NON-BLOCK MODE : " << clientSocket << strerror(errno) << std::endl;
		close(clientSocket);
		return (-1);
	}

	connections[ clientSocket ] = Connection(clientSocket, serverConfig);
	connections[ clientSocket ].clear();

	// register to epoll
	epoll_event  event; 
	event.events = EPOLLIN;	
	event.data.fd = clientSocket;
	epoll_ctl(epollFd,  EPOLL_CTL_ADD, clientSocket , &event);
	std::cout<<"Accepting client connection "<< clientSocket <<" reigistered into epoll."<<std::endl;
	
	return connections.size();
}

bool	ConnectionManager::closeConnection(int clientSocket)
{		
	// Logger::log(LC_MINOR_NOTE, "Closing client socket #%d, unregistererd from epoll", clientSocket);

	// std::map<int,Connection>::iterator it = connections.find(clientSocket);
	// epoll_ctl(epollSocket , EPOLL_CTL_DEL , clientSocket, NULL);
	// close(clientSocket);	
	// if(it != connections.end())
	// 	connections.erase(clientSocket);		
	return (clientSocket);
}

int	ConnectionManager::addRawServer(ServerConfig server)
{
	rawServers.push_back(server);
	return rawServers.size();
}

std::vector<ServerConfig>  ConnectionManager::getRawServers()
{
	return rawServers;
}

void ConnectionManager::setEpollFd(int epollscoket)
{
	epollFd = epollscoket;
}

int	 ConnectionManager::getEpollFd()
{

	return (epollFd);
}

void ConnectionManager::Info()
{
	std::cout << "\n==========================\nConnectionController\n==========================" << std::endl;
	std::cout << " -rawServers " << std::endl;
	for( std::vector<ServerConfig>::iterator it = rawServers.begin(); it != rawServers.end(); ++it)
	{
		std::cout << " server port " << it->getPort() << std::endl; 
		std::cout << " server name " << it->getServerName() << std::endl; 
	}
}


Connection *ConnectionManager::findConnection(int socket)
{
	std::map<int,Connection>::iterator it = connections.find(socket);
	if(it != connections.end())
		return &(it->second);	
	return (NULL);
}

size_t	ConnectionManager::checkTimeExpiedConnection()
{
	time_t		now = time(NULL);
	size_t 		count = 0;

	for ( std::map<int, Connection>::iterator it = connections.begin(); it != connections.end(); ++it)
	{
		if(it->second.isExpired(now) )
		{
			std::cout<<"Connection " <<it->second.getSocket()<<"timeout, closing connection and remove from epoll" <<std::endl;
			closeConnection(it->second.getSocket());
			count ++;
		}
	}
	return (count);
}
