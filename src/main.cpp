

#include "../include/HttpRequest.hpp"


int main(int argc, char **argv) 
{
    (void)argc;
    (void)argv;
    


   
    std::cout << "\n\n===== test parse request =====\n";
    char bufferReadRequest[] =
            "POST /search?q=webserv&lang=en HTTP/1.1\r\n"
            "Host: localhost:9091\r\n"
            "User-Agent: MakefileTest/1.0\r\n"
            "Accept: \r\n"
            "Cookie: PHPSESSID=xyz789; theme=dark\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 45\r\n"
            "\r\n"
            "{\"query\": \"webserv\", \"lang\": \"en\"}";
    HttpRequest     request;
    request.HttpRequest::parseHttpRequest(bufferReadRequest, request);
    
    std::cout << GREEN;
    HttpRequest::printHttpRequest(request);
    std::cout << GREEN << RESET;


    std::cout << "test public  method:" << request.method << "\n";


    return 0;
}


