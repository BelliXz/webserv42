

#include "../../include/All.hpp"

void ft_setNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


int ft_setEpoll(std::map<int, std::vector<ServerConfig> > &serversByPort,
                std::map<int, int> &portToFd,
                int &epoll_fd,
                epoll_event *events) 
{
    (void)events;
    epoll_fd = epoll_create1(0);
    epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));

    std::cout << "epoll system Ready(epoll_fd=" << epoll_fd << ")\n";
       
    for (std::map<int, std::vector<ServerConfig> >::iterator it = serversByPort.begin(); 
        it != serversByPort.end(); 
        ++it) 
    {
        int port = it->first;

        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(server_fd, (sockaddr *)&addr, sizeof(addr));
        listen(server_fd, 10);
        ft_setNonBlocking(server_fd);

        ev.events = EPOLLIN;
        ev.data.fd = server_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

        portToFd[port] = server_fd;
        std::cout << "Listening on port: " <<GREEN<< port <<RESET<< " (fd=" << server_fd << ")\n";
    }
    std::cout << "\n";

    return 0;
}

