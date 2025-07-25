
#include "Server.hpp"

Server::Server() 
{
  
}

Server::~Server()
{
	std::cout<< "<<<<<<<<<<   Cleaning up socket   >>>>>>>>>>"<<std::endl;
	for( std::vector<int>::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it)
	{
		close(*it);
	}
}

size_t	Server::parseConfig(std::string config_file)			
{
	serverconfigs = ConfigParser::parseAllConfigs(config_file);
	if (ConfigParser::checkErrorParseAllConfigs(serverconfigs)) 
	{
		std::cerr <<RED "ConfigParser: Invalid config detected. Exiting." RESET<< std::endl;
		exit(EXIT_FAILURE);
	}
	// Logger::log(LC_MINOR_NOTE, "Done parsing file with %d servers", serverconfigs.size()) ;
	return (serverconfigs.size());
}


Server::Server(std::string config_file):configFile(config_file)
{	
	// Logger::log(LC_SYSTEM , "Server instance created with config file as %s" , std::string(config_file).c_str());
	parseConfig(config_file);									
}



const std::vector<ServerConfig>& Server::getServerConfigs() const 
{
	return serverconfigs;
}

std::vector<ServerConfig>& Server::GetServerConfigs()
{
	return serverconfigs;
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

bool Server::start(ConnectionManager& cm)
{
    std::cout<< "<<<<<<<<<<   Server start   >>>>>>>>>>"<<std::endl;


    std::set<int>   used_ports;
	int				done = 0;
	

    for( std::vector<ServerConfig>::iterator it = serverconfigs.begin(); it != serverconfigs.end(); ++it)
    {
		std::cout<< "<<<<<<<<<<   Server start for loop   >>>>>>>>>>"<<std::endl;
        int	current_port = it->getPort();

		cm.addRawServer(*it);
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
		done++;
    }

    //Set up multiple servers with different hostnames (use something like: curl --resolve example.com:80:127.0.0.1 http://example.com/).
	std::map<int, ServerConfig> temp = cm.getServers();

	return (done > 0);

}

int Server::run()
{
	std::cout<< "<<<<<<<<<<   Server run   >>>>>>>>>"<<std::endl;
	ConnectionManager cm;
	
	//Create epoll 
	int epoll_fd = epoll_create1(0);
	
	start(cm);

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

	// HttpResponse 	httpResponse; 
	HttpRequest 	httpRequest;
	time_t servtimeOut = time(0) + 10;
	while (true)
	{
		// if(WEBS_DEBUG_RUN_10_SECS && time(0) > serviceExpires)
		// 	break; 

		//nfds = epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
		int nfds = epoll_wait(epoll_fd, events, SERV_MAX_EVENTS, SERV_WAIT_TIMEOUT);
		// try to continue
		if(nfds == 0)
			continue;
		// nfds error
		if (ndfs == -1)
			throw std::runtime_error("Error epoll_wait" + std::string(strerror(errno)));
		for (int i=0; i < nfds; i++)
		{
			int activeFd = events[i].data.fd;
			ServerConfig *server = cm.getServers(events[i].data.fd)
			std::cout<<"Epoll event active od fd"<< activeFd <<std::endl;


			//check server fds
			if(isServerSocket(activeFd))
			{
				// error handling
				//Generic socket error || Remote shutdown of read stream || Hang-up (e.g. client disconnected)
				if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLHUP))
				{
					std::cout<<"Error abort listening."<< events[i].data.fd<<std::endl;

					int err_code;
					socklen_t len = sizeof(err_code);
					getsockopt(activeFd, SOL_SOCKET, SO_ERROR, &err_code, &len);
					std::cerr << "getsockopt failed on fd " << activeFd << ": "<< strerror(errno) << std::endl;
					close(events[i].data.fd);
					epoll_ctl(epoll_fd, FPOLL_CTL_DEL, events[i].data.fd , NULL);
					continue;
				}

				//coming new request
				{						
					if(!server)
						throw std::runtime_error("ERROR Unable to load server configuration for fd....");
									
					struct sockaddr_in client_address;	
					socklen_t len = sizeof(client_address);
						
					int	client_socket = accept(events[i].data.fd, (struct sockaddr *)&client_address , &len);						
					if(client_socket < 0)
						throw std::runtime_error("Unable to accept()");
				//	cm.openConnection(client_socket, *server);
					continue;
				}
			}

			//check client fds
			{

			}


		}

	}

	
	return(1);

}



