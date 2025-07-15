#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include <signal.h>
#include "ServerConfig.hpp"
#include "ConnectionController.hpp"
#include "FileHandler.hpp"

#define SERV_MAX_CONNS 100 //Maximum queue length specifiable by listen.



class Server {

private:
    std::vector<ServerConfig>   serverConfigs;
    std::vector<int>            serverSockets;
    bool                        serverRun;
    ConnectionController		connectionController; 

public:
    Server();
    ~Server();
    
    void start(const std::vector<ServerConfig> &serverConfigs);
    void run();
    void stop();

};

#endif
