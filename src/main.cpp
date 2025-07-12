
#include "../include/All.hpp"


int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        std::cerr << "Usage: ./a.out config.conf\n";
        return 1;
    }

    std::map<int, std::vector<ServerConfig> > serversByPort;
    std::map<int, int> portToFd;
    int epoll_fd;
    epoll_event events[MAX_EVENTS];

    if (ft_parseConfigAndSetEpoll(argv, serversByPort, portToFd, epoll_fd, events))
        return 1;

    ft_serverLoop(epoll_fd, serversByPort, portToFd, events);

    return 0;
}


