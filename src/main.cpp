
#include "../include/All.hpp"

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        std::cerr <<RED "Usage:  ./webserv  config/xxx.conf" RESET<< std::endl;
        return 1;
    }
    std::map<int, std::vector<ServerConfig> > serversByPort;
    // try 
    // {
        serversByPort = ft_parseConfigFile(argv[1]);
    // } 
    // catch (const std::exception &e) 
    // {
    //     std::cerr << "Error parsing config: " << e.what() << "\n";
    //     return 1;
    // }


    std::map<int, int>  portToFd;
    int                 epoll_fd;
    epoll_event         events[MAX_EVENTS];
    if (ft_setEpoll(serversByPort, portToFd, epoll_fd, events))
        return 1;

        
    ft_serverLoop(epoll_fd, serversByPort, portToFd, events);
    return 0;
}


