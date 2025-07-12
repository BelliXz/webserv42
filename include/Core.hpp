

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


#include <sstream>
#include <iostream>
#include <cstring>
#include <unistd.h> // สำหรับ write/send
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>   // สำหรับ atoi
#include <iomanip>

#include <fstream>
#include <cstdio>      // for std::remove
#include <sys/socket.h> // send()
#include <cctype>



// #define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 4096


// ข้างล่างสุดมี
// #define HTML_200_TEMPLATE 
// #define HTML_404_TEMPLATE 



#define HTML_200_TEMPLATE \
"<!DOCTYPE html>\n" \
"<html lang='en'>\n" \
"<head>\n" \
"  <meta charset='UTF-8'>\n" \
"  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n" \
"  <title>Webserv Home</title>\n" \
"  <style>\n" \
"    body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 0; padding: 0; }\n" \
"    .container { max-width: 800px; margin: 50px auto; padding: 20px; background: #fff; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n" \
"    h1 { color: #333; }\n" \
"    p { color: #666; }\n" \
"  </style>\n" \
"</head>\n" \
"<body>\n" \
"  <div class='container'>\n" \
"    <h1>Welcome to Webserv 🎉 !!!!jgkvedbjkskjfvbeajk</h1>\n" \
"    <p>This is a multi-line HTML response served using epoll HTTP server in C++.</p>\n" \
"    <p>Enjoy coding and stay curious!</p>\n" \
"  </div>\n" \
"</body>\n" \
"</html>\n"


#define HTML_404_TEMPLATE \
"<!DOCTYPE html>\n" \
"<html lang='en'>\n" \
"<head>\n" \
"  <meta charset='UTF-8'>\n" \
"  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n" \
"  <title>404 Not Found</title>\n" \
"  <style>\n" \
"    body {\n" \
"      background-color: #0f172a;\n" \
"      color: #fff;\n" \
"      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n" \
"      display: flex;\n" \
"      align-items: center;\n" \
"      justify-content: center;\n" \
"      height: 100vh;\n" \
"      margin: 0;\n" \
"    }\n" \
"    .box {\n" \
"      text-align: center;\n" \
"    }\n" \
"    h1 {\n" \
"      font-size: 5rem;\n" \
"      color: #f43f5e;\n" \
"    }\n" \
"    p {\n" \
"      font-size: 1.5rem;\n" \
"      margin-bottom: 20px;\n" \
"      color: #cbd5e1;\n" \
"    }\n" \
"    a {\n" \
"      color: #38bdf8;\n" \
"      text-decoration: none;\n" \
"      font-weight: bold;\n" \
"    }\n" \
"    a:hover {\n" \
"      text-decoration: underline;\n" \
"    }\n" \
"  </style>\n" \
"</head>\n" \
"<body>\n" \
"  <div class='box'>\n" \
"    <h1>404</h1>\n" \
"    <p>Oops! Page not found.</p>\n" \
"    <a href='/'>Go to homepage</a>\n" \
"  </div>\n" \
"</body>\n" \
"</html>\n"



