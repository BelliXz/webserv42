
#include "../include/Server.hpp"

int main(int argc, char **argv)
{
 
    std::cout  <<RESET           "              set color at RouteConfig.hpp";
    std::cout  <<FD_COLOR      "\n              FD_COLOR        MAGENTA";
    std::cout  <<REQUEST_COLOR "\n              REQUEST_COLOR   BLUE";
    std::cout  <<SERVER_COLOR  "\n              SERVER_COLOR    YELLOW";
    std::cout  <<MONITOR_COLOR "\n              MONITOR_COLOR   GREEN";
    std::cout  <<ERROR_COLOR    "\n              ERROR_COLOR     RED";
    std::cout  <<RESPONSE_COLOR "\n              RESPONSE_COLOR  MAGENTA";
    std::cout  <<RESET         "\n";
    if (argc != 2) {
        std::cerr <<RED "Usage:  ./Server  <config file>" RESET<< std::endl;
        return 1;
    }
    signal(SIGPIPE, SIG_IGN);
    Server     webserver(argv[1]);
    // ConfigParser::printAllServerConfig(webserver.getServerConfigs());    // print all config file
    // ConfigParser::printSelectedServer(&webserver.getServerConfigs()[2]); // print manual  
    webserver.run();
    return 0;
}

