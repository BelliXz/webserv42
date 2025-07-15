


#include "../include/HttpRequest.hpp"
#include "../include/ServerConfig.hpp"



int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        std::cerr <<RED "Usage:  ./webserv  config/xxx.conf" RESET<< std::endl;
        return 1;
    }
    std::map<int, std::vector<ServerConfig> > serversByPort;
    try 
    {
        serversByPort = ft_parseConfigFile(argv[1]);
    } 
    catch (const std::exception &e) 
    {
        std::cerr << "Error parsing config: " << e.what() << "\n";
        return 1;
    }




    
    return 0;
}


