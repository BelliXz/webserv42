
#include "../include/Webserv.hpp"
#include "../include/HttpRequest.hpp"

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        std::cerr <<RED "Usage:  ./webserv  <config file>" RESET<< std::endl;
        return 1;
    }


    std::cout <<RED "\n\n===== test1 parse config (GET format)=====\n" RESET;
    Webserv     webserv(argv[1]);  
    Webserv::printServerConfigs(webserv.getServerConfigs());

   
    

    std::cout <<RED "\n\n===== test2 parse config (Free style)=====\n" RESET;
    std::vector<ServerConfig>	    serverConfigs;	
	serverConfigs = ConfigParser::parseAllConfigs(argv[1]);
    if (ConfigParser::checkErrorParseAllConfigs(serverConfigs)) 
	{
		std::cerr <<RED "ConfigParser: Invalid config detected. Exiting." RESET<< std::endl;
		exit(EXIT_FAILURE);
	}
    ConfigParser::printConfigParser(serverConfigs);



    
    std::cout <<RED "\n\n===== test3 parse request =====\n" RESET;
    char bufferReadRequest[] =
            "POST /search?q=webserv&lang=en HTTP/1.1\r\n"
            "Host: localhost:9091\r\n"
            "User-Agent: MakefileTest/1.0\r\n"
            "Accept: \r\n"
            "Cookie: PHPSESSID=xyz789; theme=dark\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 45\r\n"
            "\r\n"\
            "{\"query\": \"webserv\", \"lang\": \"en\"}";
    HttpRequest     request;
    request.HttpRequest::parseHttpRequest(bufferReadRequest, request);
    HttpRequest::printHttpRequest(request);
    std::cout <<GREEN "test public variable method: " RESET<< request.method << "\n";
    return 0;
}


