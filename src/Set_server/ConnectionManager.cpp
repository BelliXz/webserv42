
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


ServerConfig	*ConnectionManager::getServer(int fd)
{
	for( std::map<int, ServerConfig>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		if(fd == it->first)
		return &(it->second);
	}
	return NULL;
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
	epoll_ctl(epollSocket,  EPOLL_CTL_ADD, clientSocket , &event);
	std::cout<<"Accepting client connection "<< client_addr <<" reigistered into epoll."<<std::endl;
	
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

Connection *ConnectionManager::findConnection(int socket)
{
	std::map<int,Connection>::iterator it = connections.find(socket);
	if(it != connections.end())
		return &(it->second);	
	return (NULL);
}
