
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


bool	Server::isServerSocket(int socket)
{
	for (std::vector<int>::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it)
		if (*it == socket)
			return (true);
	return (false);
}

bool Server::start(ConnectionManager& cm)
{
    // std::cout<< "<<<<<<<<<<   Server start   >>>>>>>>>>"<<std::endl;
    std::set<int>   used_ports;
	int				done = 0;
	
    for( std::vector<ServerConfig>::iterator it = serverconfigs.begin(); 
		it != serverconfigs.end(); 
		++it)
    {
		// std::cout<< "<<<<<<<<<<   Server start for loop   >>>>>>>>>>"<<std::endl;
        int	current_port = it->getPort();
		cm.addRawServer(*it);
		std::cout << "start 136 Server started on port: " << SERVER_COLOR <<  current_port << RESET;
		std::cout << "\n";
        if (used_ports.find(current_port) != used_ports.end())
		{
			std::cout<< "start 138 Port is already bound"<<std::endl;
			continue;
		}
		std::cout<< "start 177 process serverSockets.push_back(serverSocket)   add fd"<<std::endl;
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
			{
				std::cerr << "171 error bind" << std::endl;
                throw std::runtime_error("Failed to bind on server");
			}
            // listen Maximum queue length specifiable by listen.
			if (listen(serverSocket, SERV_MAX_CONNS) < 0)
			{
				std::cerr << "176 error listen"<< std::endl;
                throw std::runtime_error("Failed to listen on server" + std::string(strerror(errno)));
			}
			it->setPortFd(serverSocket);
            serverSockets.push_back(serverSocket);
            used_ports.insert(current_port);
			std::cout<< "start 189 Server started on port: " << SERVER_COLOR <<  current_port << RESET;// kit add
			std::cout<< "	serverSocket (fd) on : " <<FD_COLOR <<  serverSocket << RESET;// kit add
			std::cout<< RESET<< std::endl;
			std::cout<< RESET<< std::endl;
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
	// std::cout<< "<<<<<<<<<<   Server run   >>>>>>>>>"<<std::endl;
	// ConnectionManager cm;
	
	//Create epoll 
	int epoll_fd = epoll_create1(0);
	
	start(cm);

	cm.setEpollFd(epoll_fd);
	if (epoll_fd == -1) 
		throw std::runtime_error("Error creating epoll instance");
	struct epoll_event		events[SERV_MAX_EVENTS];
	memset( events, 0 , sizeof(events));


	std::map<int, ServerConfig*> serverFdMap;
	for (size_t i = 0; i < serverSockets.size(); ++i)
	{
		int fd = serverSockets[i];
		events[i].events = EPOLLIN;
		events[i].data.fd = fd;

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &events[i]) < 0)
			throw std::runtime_error("epoll_ctl error");
	
		for (size_t j = 0; j < serverconfigs.size(); ++j)
		{
			std::cout << "		run 295 serverconfigs[j].getPortFd() = "<< serverconfigs[j].getPortFd() ;
			std::cout << "	fd = " << fd ;
			
			if (serverconfigs[j].getPortFd() == fd)
			{
				serverFdMap[fd] = &serverconfigs[j];
				std::cout << "	Add";
				std::cout << "\n";
				break;
			}

			std::cout << "\n";
		}
	}

	// HttpRequest 	httpRequest;
	//time_t servtimeout = time(0) + 10;
	while (true)
	{
		//if(WEBS_DEBUG_RUN_10_SECS && time(0) > serviceExpires)
		// if(time(0) > servtimeout)
		// 	break; 

		//nfds = epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
		std::cout<< "		run 276 epoll_wait"; //<<std::endl;
		int nfds = epoll_wait(epoll_fd, events, SERV_MAX_EVENTS, SERV_WAIT_TIMEOUT);
		// try to continue
		if(nfds < 1)
			std::cout<< "		nfds = " << nfds << std::endl;	
		if(nfds > 0)
			std::cout<<MONITOR_COLOR <<"		nfds = " << nfds << RESET<< std::endl;

		// nfds error
		if (nfds == -1)
			throw std::runtime_error("Error epoll_wait" + std::string(strerror(errno)));
		for (int i=0; i < nfds; i++)
		{
			int activeFd = events[i].data.fd;
			ServerConfig *server = NULL;

			if (serverFdMap.count(events[i].data.fd))
				server = serverFdMap[events[i].data.fd];
			if (server)
			{
				std::cout << "		run 292 Active fd# " << FD_COLOR <<  activeFd << RESET;
				std::cout << " is a server socket, server name: " << SERVER_COLOR << server->getServerName() << RESET << std::endl; 
			}
			else
			{
				std::cout << "		run 296 Active fd# " <<FD_COLOR << activeFd << RESET<< " is a client socket" << std::endl;  
			}
			std::cout<< "		run 310 Epoll event " << FD_COLOR << "active fd	: "<<FD_COLOR << activeFd << RESET<<std::endl;

			if(isServerSocket(activeFd))
			{
				// std::cout<< "<<<<<<<<<<   Servers checking...   >>>>>>>>>"<<std::endl;
				// error handling
				//Generic socket error || Remote shutdown of read stream || Hang-up (e.g. client disconnected)
				if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLHUP))
				{
					std::cout<< "Error abort listening."<< events[i].data.fd<<std::endl;

					int err_code;
					socklen_t len = sizeof(err_code);
					getsockopt(activeFd, SOL_SOCKET, SO_ERROR, &err_code, &len);
					std::cerr << "getsockopt failed on fd " << activeFd << ": "<< strerror(errno) << std::endl;
					close(events[i].data.fd);
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd , NULL);
					continue;
				}

		
				if(!server)
				{
					std::cout<<GREEN;
					std::cerr << "339 No server fd: " << activeFd << std::endl;
					std::cout<<GREEN << RESET;
					throw std::runtime_error("ERROR Unable to load server configuration for fd....");
				}
				else
				{
					std::cout<<MONITOR_COLOR;
					std::cout<<"run 346 Nave server	: " << server->getServerName() << std::endl;
					std::cout<<"run 384 Listening port	: " << server->getPort() << std::endl;
					std::cout<<"run 385 fd	 	: " << activeFd << std::endl;
					std::cout<<GREEN << RESET;
				}
				struct sockaddr_in client_addr;	
				socklen_t lenClientAddr = sizeof(client_addr);
					
				int	client_socket = accept(events[i].data.fd, (struct sockaddr *)&client_addr , &lenClientAddr);						
				if(client_socket < 0)
					throw std::runtime_error("Unable to accept()" + std::string(strerror(errno)));

				cm.openConnection(client_socket, *server);
				continue;
			}

			std::cout << 	"		run 367 " <<FD_COLOR <<"events[i].data.fd	: " << FD_COLOR <<events[i].data.fd << RESET << "\n";

			std::cout  << "		run 453 call read\n";
			cm.read(events[i].data.fd);
			std::cout  << "run 460 call read (finish)\n";


			std::cout  << "run 473 call write\n";
			cm.write(events[i].data.fd);
			std::cout  << "run 479 call write (finish)\n";

			close (activeFd); 
		}
	}

	return(1);

}


// void Server::printRoutes(const std::map<std::string, RouteConfig>& routes)
// {
// 	std::cout << "=== Printing RouteConfig Map ===" << std::endl;
// 	for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it)
// 	{
// 		std::cout << "=== Printing RouteConfig Map ===" << std::endl;
// 		const std::string& path = it->first;
// 		const RouteConfig& route = it->second;
// 		std::cout << "Route Path: " << path << std::endl;
// 		std::cout << "  Root: " << route.getRoot() << std::endl;
// 		std::cout << "  Index: " << route.getIndex() << std::endl;
// 		std::cout << "  Methods: ";
// 		std::vector<std::string> methods = route.getMethods();
// 		for (size_t i = 0; i < methods.size(); ++i)
// 		{
// 			std::cout << methods[i];
// 			if (i + 1 < methods.size()) std::cout << ", ";
// 		}
// 		std::cout << std::endl;
// 		std::cout << "-------------------------------" << std::endl;
// 	}
// }


// RouteConfig Server::selectServerAndRoute(HttpRequest& request, ServerConfig& server, std::string& fullPath)
// {
// 	std::cout << "Selecting route for: " << request.method << " " << request.path << std::endl;	
// 	const std::map<std::string, RouteConfig>& routes = server.getRoutes();
// 	RouteConfig matchRoute;
// 	size_t      longestMatch = 0;
// 	for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it)
// 	{
// 		const std::string& routePath = it->first;
// 		if (request.path.find(routePath) == 0 && routePath.length() > longestMatch)
// 		{
// 			matchRoute = it->second;
// 			longestMatch = routePath.length();
// 		}
// 	}
// 	if (matchRoute.getPath().empty())
// 	{
// 		std::cerr << RED << "No matching route for path: " << request.path << RESET << std::endl;
// 		throw std::runtime_error("No matching route found");
// 	}
// 	// สร้าง fullPath: root + ส่วนที่เหลือจาก path
// 	fullPath = matchRoute.getRoot();
// 	std::string remaining = request.path.substr(matchRoute.getPath().length());
// 	if (!remaining.empty() && remaining[0] == '/')
// 		fullPath += remaining;
// 	else
// 		fullPath += "/" + remaining;
// 	std::cout << "Matched route: " << matchRoute.getPath() << std::endl;
// 	std::cout << "Full file path: " << fullPath << std::endl;
// 	return matchRoute;
// }
