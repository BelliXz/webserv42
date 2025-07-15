#include "Server.hpp"
#include "ConnectionController.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <vector>

Server::Server() : serverSockets(), serverConfigs(), connectionController(), serverRun(true) 
{
    signal(SIGPIPE, SIG_IGN);
}

Server::~Server() 
{
    for (size_t i = 0; i < serverSockets.size(); ++i) {
        close(serverSockets[i]);
    }
}

void Server::start(const std::vector<ServerConfig> &serverConfigs)
{
    std::cout<< "Server start"<<std::endl;
    this->serverConfigs = serverConfigs;
    connectionController.setConfigs(serverConfigs);

    for(size_t i =0; i < serverConfigs.size(); ++i)
    {

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
            socketAddr.sin_port = htons(serverConfigs[i].port);

            // binding with port
			if (bind(serverSocket, (struct sockaddr*)&socketAddr , sizeof(socketAddr) ) < 0)
                throw std::runtime_error("Failed to bind on server");

            // listen Maximum queue length specifiable by listen.
			if (listen(serverSocket, SERV_MAX_CONNS) < 0)
                throw std::runtime_error("Failed to listen on server");
        }

    }

}
