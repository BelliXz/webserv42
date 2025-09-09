
#include "ConnectionManager.hpp"
#include "Server.hpp"

int ConnectionManager::epollFd = 0; 

ConnectionManager::ConnectionManager()
{
	// std::cout<< "ConnectionManage  has been called"<<std::endl;
}

ConnectionManager::~ConnectionManager()
{
    std::cout<< "ConnectionManage destructor has been called"<<std::endl;
    for(std::map<int, Connection>::iterator it = connections.begin(); it!= connections.end(); ++it )
		closeConnection(it->first);
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


Connection *ConnectionManager::findConnection(int socket)
{
	std::map<int,Connection>::iterator it = connections.find(socket);
	if(it != connections.end())
		return &(it->second);	
	return (NULL);
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
	std::cout<<"		cm 52 Accepting client connection "<<FD_COLOR << clientSocket << RESET<<" reigistered into epoll."<<std::endl;
	
	return connections.size();
}


bool ConnectionManager::read(int clientSocket)
{
    std::cout << "		read 133 events[i].data.fd : clientSocket :"
              << FD_COLOR << clientSocket << RESET << "\n";

    Connection *conn = NULL;
    for (std::map<int, Connection>::iterator it = connections.begin(); 
											 it != connections.end(); 
											 ++it) {
        if (it->first == clientSocket) {
            conn = &(it->second);
			
			std::cout << MONITOR_COLOR ;
			std::cout << "		read 164 (option) call printConnection\n" << RESET;
			std::cout  << MONITOR_COLOR << RESET;
            conn->printConnection();
			
			ServerConfig const test = conn->getServerConfig();
			(void)test;
			std::cout  << MONITOR_COLOR <<" 		read 180 (option) call printSelectedServer \n";
			std::cout  << MONITOR_COLOR << RESET;
			// Server::printSelectedServer(&test);
            break;
        }
    }
    if (!conn) {
        std::cout << "117 haven't Connection *conn\n";
        return false;
    }

    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytes <= 0 || strlen(buffer) == 0) {
        close(clientSocket);
        connections.erase(clientSocket);
        return false;
    }


	HttpRequest     request;
	request.HttpRequest::parseHttpRequest(buffer, request);
    try {
		std::cout  << MONITOR_COLOR << "\n\n\nread 189 call append\n" << RESET; 
        bool done = conn->appendRequestBufferAndProcess(buffer,request,bytes,rawServers);

		std::cout  << MONITOR_COLOR << "\n\n\n		read 193 call response\n" << RESET; 
		conn->handleHttpResponse(request, rawServers, bytes);
		
		(void)done;
        return done;
    }
    catch (const RequestException &e) {
		std::cout << RED;
		std::cout << "\n\ncatch from read =====> e.what = " ;
		std::cerr << e.what() << '\n';
		std::cout << "===============================================\n\n" ;
		std::cout << RED << RESET;

        // HttpResponse errorResp;
        // errorResp.setStatus(e.getStatusCode());
        // errorResp.setBody(HttpResponse::getErrorPage(e.getStatusCode(), conn->getServerConfig()));
        // conn->ready(errorResp, true);
    }
    return true;
}



bool	ConnectionManager::write(int clientSocket )
{
	Connection *conn = findConnection(clientSocket);
	size_t sendSize = 4096;
    conn->sendResponse();
		
	
	(void)conn;
	(void)sendSize;
	
	return (true);
}


