
#include "Server.hpp"

size_t	Server::parseConfig(std::string config_file)			
{
	serverConfigs = ConfigParser::parseAllConfigs(config_file);
	if (ConfigParser::checkErrorParseAllConfigs(serverConfigs)) 
	{
		std::cerr <<RED "ConfigParser: Invalid config detected. Exiting." RESET<< std::endl;
		exit(EXIT_FAILURE);
	}
	// Logger::log(LC_MINOR_NOTE, "Done parsing file with %d servers", serverConfigs.size()) ;
	return (serverConfigs.size());
}


Server::Server(std::string config_file):configFile(config_file)
{	
	// Logger::log(LC_SYSTEM , "Server instance created with config file as %s" , std::string(config_file).c_str());
	parseConfig(config_file);									
}

const std::vector<ServerConfig>& Server::getServerConfigs() const 
{
	return serverConfigs;
}

void Server::printServerConfigs(const std::vector<ServerConfig>& servers) 
{
	for (size_t i = 0; i < servers.size(); ++i) 
	{
		const ServerConfig& srv = servers[i];
		std::cout << "---- Server [" << i << "] --------\n";
		std::cout << GREEN;
		std::cout << "Port: " << srv.getPort() << " =============\n";
		std::cout << GREEN << RESET;
		std::cout << "Host: " << srv.getHost() << "\n";
		std::cout << "Server Name: " << srv.getServerName() << "\n";
		std::cout << "Root: " << srv.getRoot() << "\n";
		std::cout << "Index: " << srv.getIndex() << "\n";
		std::cout << "Client Max Body Size: " << srv.getClientMaxBodySize() << "\n";

		std::cout << "Error Pages:\n";
		const std::map<int, std::string>& errPages = srv.getErrorPages();
		for (std::map<int, std::string>::const_iterator it = errPages.begin(); it != errPages.end(); ++it)
			std::cout << "  " << it->first << " => " << it->second << "\n";

		std::cout << "Routes:\n";
		const std::map<std::string, RouteConfig>& routes = srv.getRoutes();
		for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it) 
		{
			const RouteConfig& route = it->second;
			std::cout << GREEN;
			std::cout << "  Path: " << route.getPath() << "\n";
			std::cout << GREEN << RESET;
			std::cout << "    Root: " << route.getRoot() << "\n";
			std::cout << "    Index: " << route.getIndex() << "\n";
			std::cout << "    Autoindex: " << (route.getAutoindex() ? "on" : "off") << "\n";
			std::cout << "    Client Max Body Size: " << route.getClientMaxBodySize() << "\n";
			std::cout << "    Upload Store: " << route.getUploadStore() << "\n";

			// Allowed Methods
			std::cout << "    Allow Methods: ";
			const std::vector<std::string>& methods = route.getMethods();
			for (size_t j = 0; j < methods.size(); ++j) 
			{
				std::cout << methods[j];
				if (j + 1 < methods.size()) std::cout << ", ";
			}
			std::cout << "\n";

			// CGI Map
			std::cout << "    CGI Passes:\n";
			const std::map<std::string, std::string>& cgis = route.getCGIs();
			for (std::map<std::string, std::string>::const_iterator cit = cgis.begin(); cit != cgis.end(); ++cit) 
			{
				std::cout << "      " << cit->first << " => " << cit->second << "\n";
			}

			// Return Directive
			if (route.getReturnStatus() != 0) 
			{
				std::cout << "    Return: " << route.getReturnStatus() << " => " << route.getReturnValue() << "\n";
			}
		}
		std::cout << "\n";
	}
}

void Server::start(const std::vector<ServerConfig> &serverConfigs)
{
    std::cout<< "<<<<<<<<<<   Server start   >>>>>>>>>>"<<std::endl;
    this->serverConfigs = serverConfigs;
   // connectionController.setConfigs(serverConfigs);
    std::set<int>   used_ports;
	int				connet = 0;

    for( std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int	current_port = it->getPort();
		//cc.addRawServer(*it);
        

        if (used_ports.find(current_port) != used_ports.end())
		{
			std::cout<< "Port is already bound"<<std::endl;
			continue;
		} 
        
        try
        {
            //create server socket : IPv4 Internet protocols , TCP
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket < 0)
            {
                throw std::runtime_error("Failed to create server socket" + std::string(strerror(errno)));
            }
            // set to non-blocking mode
            // set the file descriptor flags to the value specified by arg.
            if (fcntl(serverSocket, F_SETFL , O_NONBLOCK) == -1)
            {
                close(serverSocket);
                throw std::runtime_error("Failed to set server to non-blocking");
            }
            // reusable socket if the server was restart before port allocation timeout
            int opt = 1;
            if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
            {
                close(serverSocket);
                throw std::runtime_error("Failed to set socket options: " + std::string(strerror(errno)));
            }

            sockaddr_in    socketAddr;
            memset(&socketAddr, 0, sizeof(sockaddr));
            socketAddr.sin_family = AF_INET;
            socketAddr.sin_addr.s_addr = INADDR_ANY;
            socketAddr.sin_port = htons(it->getPort());

            // binding with port
			if (bind(serverSocket, (struct sockaddr*)&socketAddr , sizeof(socketAddr) ) < 0)
                throw std::runtime_error("Failed to bind on server");

            // listen Maximum queue length specifiable by listen.
			if (listen(serverSocket, SERV_MAX_CONNS) < 0)
                throw std::runtime_error("Failed to listen on server" + std::string(strerror(errno)));
            

            serverSockets.push_back(serverSocket);
            used_ports.insert(current_port);

        
        }
        catch(std::exception &e)
        {
			throw std::runtime_error("Exception caught:" + std::string(strerror(errno)));
        }
    }

    //Set up multiple servers with different hostnames (use something like: curl --resolve example.com:80:127.0.0.1 http://example.com/).


}

int Server::run()
{
	std::cout<< "<<<<<<<<<<   Server run   >>>>>>>>>"<<std::endl;
	ConnectionManager cm;

	//start(cm);
	//setupSockets(cc);

	//Create epoll 
	int epoll_fd = epoll_create1(0);
	cm.setEpollFd(epoll_fd);
	if (epoll_fd == -1) 
		throw std::runtime_error("Error creating epoll instance");
	struct epoll_event		events[SERV_MAX_EVENTS];
	memset( events, 0 , sizeof(events));
	
	//add server fds into the epoll_events
	int order = 0; 
	for ( std::vector<int>::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it)
	{
		events[order].events = EPOLLIN;	
		events[order].data.fd = *it;
		if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD , *it , &events[order] ) < 0)
			throw std::runtime_error("epoll_ctl error");
		order ++;

	}

	HttpRequest 	httpRequest;
	//time_t serviceExpires = time(0) + 10;
	while (true) 
	{
			// if(WEBS_DEBUG_RUN_10_SECS && time(0) > serviceExpires)
			// 	break; 

			//int nfds = epoll_wait(epollFd, events, MAX_EVENTS, timeout);
			int nfds = epoll_wait(epoll_fd, events , SERV_MAX_EVENTS ,SERV_WAIT_TIMEOUT);
			// no effected fds, but happens from timeout
			if(nfds == 0)
				continue;
			// error stuffs
			if(nfds == -1)
				throw std::runtime_error("epoll_wait error");

			for (int i=0;i<nfds;i++)
			{
				int			active_fd = events[i].data.fd;
				ServerConfig *server = cc.getServer(events[i].data.fd);
				Logger::log(LC_MINOR_NOTE, "epoll event on fd#%d!" , active_fd);
				
				if (isServerSocket(active_fd))
				{
					// error handling
					if ((events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLHUP) || (events[i].events & EPOLLERR))
					{
						Logger::log(LC_CONN_LOG, "Listening Socket %d error, abort listening ", events[i].data.fd);
						int error_code ;
						socklen_t len = sizeof(error_code);
						getsockopt(active_fd , SOL_SOCKET, SO_ERROR , &error_code , &len);
						Logger::log(LC_ERROR, "ERROR: %s" , len);
						close(events[i].data.fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL , events[i].data.fd , NULL);
						continue;
					}

					// upcoming new request
					if(events[i].events & EPOLLIN)
					{						

					}
					// end server fds
					
				}
	
				// Start client Socket checking				
				{

					
					
				}
			}
			connectionController.purgeExpiredConnections();

	}
	// this won't be reached anyway 
	close(epoll_fd);
	return (0);
	

}



