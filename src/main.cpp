
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
    
    
    // Server::printAllServerConfig(webserver.getServerConfigs());     
    // Server::printSelectedServer(&webserver.getServerConfigs()[2]);                             
    for (size_t i = 0; i < webserver.getServerConfigs().size(); ++i) {
        if ( webserver.getServerConfigs()[i].getPort() == 4343 )
            Server::printSelectedServer(&webserver.getServerConfigs()[i]);
    }

    // Server::printAllRoutes(webserver.getServerConfigs()[0].getRoutes());                       
    // Server::printSelecteRoutes(webserver.getServerConfigs()[0].getRoutes().begin()->second);   
    for (std::map<std::string, RouteConfig>::const_iterator it = webserver.getServerConfigs()[0].getRoutes().begin();
                                                            it != webserver.getServerConfigs()[0].getRoutes().end(); 
                                                            ++it) {
        const RouteConfig& route = it->second;
        if (route.getPath() == "/session")
            Server::printSelecteRoutes(route);
    }

    webserver.run();
    return (0);
}




/*
    std::cout <<RED "\n\n===== test parse request =====\n" RESET;
    char bufferReadRequest[] =
            "POST /search?q=Server&lang=en HTTP/1.1\r\n"
            "Host: localhost:9091\r\n"
            "User-Agent: MakefileTest/1.0\r\n"
            "Accept: \r\n"
            "Cookie: PHPSESSID=xyz789; theme=dark\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 45\r\n"
            "\r\n"\
            "{\"query\": \"Server\", \"lang\": \"en\"}";
    HttpRequest     request;
    request.HttpRequest::parseHttpRequest(bufferReadRequest, request);
    HttpRequest::printHttpRequest(request);
    std::cout <<GREEN "test public variable method: " RESET<< request.method << "\n";
*/
