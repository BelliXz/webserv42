# define BOLD 	"\033[1m"
# define RED 	"\033[0;31m"
# define GREEN 	"\033[0;32m"
# define BLUE	"\033[0;34m"
# define CYAN 	"\033[0;36m"
# define GRAY 	"\033[0;37m"
# define LRED 	"\033[0;91m"
# define YELLOW	"\033[0;33m"
# define MAGENTA "\033[0;35m" 
# define RESET  "\033[0m"
# define RGB 	"\033[1m\033[38;2;255;182;193m"




#include <iostream>
#include <map>
#include <vector>
#include <string>

// ===== Classes =====
class ServerConfig {
public:
    int             port;
    std::string     serverName;

    ServerConfig() 
                : port(0)   , serverName("default") {}
    ServerConfig(  int p    , const std::string &name) 
                : port(p)   , serverName(name) {}
};
class Connection {
public:
    int             fd;
    ServerConfig    serverConfig;

    Connection() 
              : fd(-1) {}
    Connection(int fd_  , const ServerConfig &conf) 
              : fd(fd_) , serverConfig(conf) {}
};
class ConnectionController {
public:
    std::map<int, Connection>       connections;  // ok
    std::map<int, ServerConfig>     servers;
    std::vector<ServerConfig>       rawServers;//ok
};

int main() {
    std::cout << "==== test build objects ====\n" ;
    // สร้าง ServerConfig หลายตัว
    std::vector<ServerConfig>   vec;
    vec.push_back(ServerConfig(8888, "server8888"));
    vec.push_back(ServerConfig(9999, "server9999"));
    vec.push_back(ServerConfig(7777, "server7777"));
    std::cout <<YELLOW "Print vector<ServerConfig>\n" RESET;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "vec[" << i 
                  << "] port=" << vec[i].port 
                  << "  name=" << vec[i].serverName << "\n";
    }
    // ใช้ iterator print
    std::cout <<YELLOW "Print vector<ServerConfig> with iterator:\n" RESET;
    for (std::vector<ServerConfig>::iterator it = vec.begin(); 
                                             it != vec.end(); 
                                             ++it) {
        std::cout << "port=" << it->port 
                  << "  name=" << it->serverName << "\n";
    }

    // สร้าง class  ServerConfig หลายตัว
    ConnectionController cc;
    cc.rawServers.push_back(ServerConfig(8080, "server8080"));
    cc.rawServers.push_back(ServerConfig(9090, "server9090"));  
    cc.rawServers.push_back(ServerConfig(7070, "server7070"));
    cc.rawServers.push_back(ServerConfig(7070, "server5050"));
    std::cout <<RED "\nPrint  ConnectionController     cc.rawServers[i].port    .serverName;\n" RESET;
    for (size_t i = 0; i < cc.rawServers.size(); ++i) {
        std::cout << "vec[" << i 
                  << "] port=" << cc.rawServers[i].port 
                  << "  name=" << cc.rawServers[i].serverName << "\n";
    }
    std::cout <<RED "Print  with iterator     cc.rawServers.begin()   .end()\n" RESET;
    for (std::vector<ServerConfig>::iterator it = cc.rawServers.begin(); 
                                             it != cc.rawServers.end(); 
                                             ++it) {
        std::cout << "port=" << it->port 
                  << "  name=" << it->serverName << "\n";
    }


    
    // ทดลอง map<int, ServerConfig>
    //std::map<int, ServerConfig>     servers;  // class ConnectionController         
    std::map<int, ServerConfig>     map_servers;   
    map_servers[vec[0].port] = vec[0];
    map_servers[vec[1].port] = vec[1];
    map_servers[vec[2].port] = vec[2];
    map_servers[00] = vec[0];
    map_servers[11] = vec[1];
    map_servers[22] = vec[2];
   
    std::cout <<GREEN "\nPrint map<int, ServerConfig>:\n" RESET;
    for (std::map<int, ServerConfig>::iterator  it = map_servers.begin(); 
                                                it != map_servers.end(); 
                                                ++it) {
        std::cout << "key=" << it->first 
                  << "  =>(port=" << it->second.port 
                  << "  ,name=" << it->second.serverName << ")\n";
    }
    // ทดลอง ConnectionController
    ConnectionController aa;
    // aa.rawServers = vec; // copy vector ทั้งหมดเข้า rawServers
    // add connections
              // class Connection fd  serverConfig
    aa.connections[1] = Connection(1, vec[0]);
    aa.connections[2] = Connection(2, vec[1]);

    std::cout <<BLUE "\nPrint Connection  ///copy vector ทั้งหมดเข้า rawServers\n" RESET;
    for (std::map<int, Connection>::iterator it = aa.connections.begin(); 
         it != aa.connections.end(); ++it) {
        std::cout << "fd="                  << it->first 
                  << "  =>(server port="    << it->second.serverConfig.port 
                  << "  ,name="             << it->second.serverConfig.serverName << ")\n";
    }


    return 0;
}
