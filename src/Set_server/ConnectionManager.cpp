
#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager(){}

ConnectionManager::~ConnectionManager()
{
    std::cout<< "ConnectionManage destructor has been called"<<std::endl;
    for(std::map<int, Connection>::iterator it = connections.begin(); it!= connections.end(); ++it )
		closeConnection(it->first);
}

bool	ConnectionManager::closeConnection(int clientSocket)
{		
	// Logger::log(LC_MINOR_NOTE, "Closing client socket #%d, unregistererd from epoll", clientSocket);

	// std::map<int,Connection>::iterator it = connections.find(clientSocket);
	// epoll_ctl(epollSocket , EPOLL_CTL_DEL , clientSocket, NULL);
	// close(clientSocket);	
	// if(it != connections.end())
	// 	connections.erase(clientSocket);		
	return (true);
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

void ConnectionManager::setEpollFd(int epollFd)
{
	epollFd = epollFd;
}

int ConnectionManager::getEpollFd()
{

	return (epollFd);
}
