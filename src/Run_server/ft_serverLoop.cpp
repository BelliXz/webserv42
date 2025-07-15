
#include "../../include/All.hpp"

int   ft_checkLeakFd()
{
    std::string     cmd;
    cmd.clear();
    std::cout << "\nIf <";
    std::cout << CYAN;
    std::cout << "make fl";
    std::cout << CYAN << RESET;
    std::cout << "> and type <" ;
    std::cout << CYAN;
    std::cout << "exit";
    std::cout << CYAN << RESET;
    std::cout << "> ==> for check leak ,fd(exit program)\n";
    std::cout << "or \n";
    std::cout << CYAN;
    std::cout << "type other";
    std::cout << CYAN << RESET;
    std::cout << " ==> for run webserv\n";

    std::getline(std::cin, cmd);
    std::cout << cmd << std::endl;
    if ( cmd == "exit")
        return(1);
    else
        return (0);
}


void ft_serverLoop(int epoll_fd,
                   std::map<int, std::vector<ServerConfig> > &serversByPort,
                   std::map<int, int> &portToFd,
                   epoll_event *events) 
{
    (void)serversByPort;
    epoll_event     ev;
    std::memset(&ev, 0, sizeof(ev));

    std::cout << CYAN;
    std::cout << "If will check leak ,fd ==> open comment\n";
    std::cout << "<if(ft_checkLeakFd()==1)  Break>\n";
    std::cout << CYAN << RESET;

    while (true) 
    {
        // if (ft_checkLeakFd() == 1)
        //     break;
        int     nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) 
        {
            int     fd = events[i].data.fd;
            bool    isServerFd = false;


            for (std::map<int, int>::iterator it = portToFd.begin(); it != portToFd.end(); ++it) 
            {
                if (it->second == fd) 
                {
                    isServerFd = true;
                    break;
                }
            }
            if (isServerFd) 
            {
                int     client_fd = accept(fd, 0, 0);
                if (client_fd >= 0) 
                {
                    ft_setNonBlocking(client_fd);
                    ev.events = EPOLLIN;
                    ev.data.fd = client_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                    std::cout << "New client on fd=" << client_fd << "\n";
                }
            } 
            else 
            {
                char    buffer[BUFFER_SIZE] = {0};
                int     bytes = recv(fd, buffer, sizeof(buffer), 0); // อ่าน fd ลง buffer

                if (bytes <= 0) 
                {
                    std::cout << RED ;
                    std::cout << "epoll system monitor fd = " << fd << "\n";
                    std::cout << "Client disconnected: " << fd << "\n";
                    std::cout << RED  << RESET;

                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
                } 
                else 
                {
                    
                    std::cout << BLUE;
                    std::cout << "epoll system monitor fd = " << fd << "\n";
                    std::cout << "buffer ==> Request:\n" << buffer << std::endl;
                    std::cout << BLUE  << RESET;

                    HttpRequest     req;
                    ft_parseHttpRequest(buffer, req, fd); // แยกข้อมูลออกจาก raw HTTP request     <=== paser            
                    ft_handleClient(fd, req);
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
                }
            }
        }
    }
}

