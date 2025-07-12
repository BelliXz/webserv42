
// #include "Core.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"


// ft_checkErrorHttpRequest
int     ft_checkErrorHttpRequest(const HttpRequest& req, int client_fd);


// ft_getPostDelete
void    ft_sendResponse(int client_fd, int status_code, const std::string& status_text,
    const std::string& body, const std::string& content_type);
void    ft_handleGet(int client_fd, const HttpRequest& req); 
void    ft_handlePost(int client_fd, const HttpRequest& req); 
void    ft_handleDelete(int client_fd, const HttpRequest& req); 


// ft_redirect
void    ft_redirect(int client_fd, const std::string& location, int status_code);


// ft_handleClient
void    ft_handleClient(int client_fd, char* buffer) ;


// ft_parseHttpRequest
void    ft_parseHttpRequest(char* buffer, HttpRequest& req);
void    ft_printHttpRequest(const HttpRequest& req); 


// ft_serverLoop
int             ft_checkLeakFd();
void            ft_serverLoop(int epoll_fd,
                    std::map<int, std::vector<ServerConfig> > &serversByPort,
                    std::map<int, int> &portToFd,
                    epoll_event *events);


// ft_parseConfig
std::string     ft_trim(const std::string& str) ;
int             ft_stoi(const std::string& s) ;
std::vector<std::string>    ft_splitWord(const std::string& line) ;
std::string     ft_stripAfterSemicolon(const std::string& s);
std::map<int, std::vector<ServerConfig> > ft_parseConfigFile(const std::string& filename) ;
void            ft_printServer(const std::map<int, std::vector<ServerConfig> >& serversByPort) ;


// ft_setEpoll
void            ft_setNonBlocking(int fd) ;
int             ft_setEpoll(
                    std::map<int, std::vector<ServerConfig> > &serversByPort,
                    std::map<int, int> &portToFd,
                    int &epoll_fd,
                    epoll_event *events);

                    
// ft_sleep
void            ft_sleep(int nb);


// main
//int	main();    

