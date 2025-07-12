

#include "../../include/All.hpp"


std::string ft_trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// int port = std::stoi(portStr);
int ft_stoi(const std::string& s) 
{
    std::stringstream ss(s);
    int result;
    ss >> result;
    return result;
}

std::vector<std::string> ft_splitWord(const std::string& line) 
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

std::string ft_stripAfterSemicolon(const std::string& s) {
    size_t pos = s.find(';');
    if (pos != std::string::npos)
        return s.substr(0, pos);
    return s;
}


std::map<int, std::vector<ServerConfig> > ft_parseConfigFile(const std::string& filename) 
{
    std::ifstream file(filename.c_str());
    if (!file.is_open()) throw std::runtime_error("Unable to open config file");

    std::map<int, std::vector<ServerConfig> > servers;
    ServerConfig currentServer;
    RouteConfig currentRoute;
    bool inServer = false;
    bool inLocation = false;
    std::string line;

    while (std::getline(file, line)) 
    {
        line = ft_trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "server {") 
        {
            inServer = true;
            currentServer = ServerConfig();
            continue;
        }

        if (line == "}") 
        {
            if (inLocation) 
            {
                currentServer.routes.push_back(currentRoute);
                inLocation = false;
            } 
            else if (inServer) 
            {
                servers[currentServer.port].push_back(currentServer);
                inServer = false;
            }
            continue;
        }

        if (!inServer) continue;

        if (line.find("location") == 0) 
        {
            inLocation = true;
            currentRoute = RouteConfig();
            std::vector<std::string> parts = ft_splitWord(line);
            if (parts.size() >= 2)
                currentRoute.path = parts[1];
            continue;
        }

        std::vector<std::string> tokens = ft_splitWord(line);
        if (tokens.empty()) continue;

        // ---------- Inside Location Block ----------
        if (inLocation) 
        {
            std::string key = tokens[0];

            if (key == "root")
            {   
                currentRoute.root = ft_stripAfterSemicolon(tokens[1]);
                // std::cout <<RED "root   = " <<  currentRoute.root << RESET<< "\n";
            }
            else if (key == "index")
                currentRoute.index = ft_stripAfterSemicolon(tokens[1]);
            else if (key == "client_max_body_size")
                currentRoute.clientMaxBodySize = ft_stoi(ft_stripAfterSemicolon(tokens[1]));
            else if (key == "upload_store")
            {   
                currentRoute.uploadStore = ft_stripAfterSemicolon(tokens[1]);
                // std::cout <<RED "Upload = " <<  currentRoute.uploadStore << RESET<< "\n";
                 
            }
            else if (key == "autoindex")
                currentRoute.autoIndex = (tokens[1].find("on") != std::string::npos);
            else if (key == "allowed_methods") 
            {
                for (size_t i = 1; i < tokens.size(); ++i) 
                    currentRoute.methods.push_back(ft_stripAfterSemicolon(tokens[i]));
            } 
            else if (key == "cgi_pass" && tokens.size() >= 3) 
                currentRoute.cgiExtensions[ft_stripAfterSemicolon(tokens[1])] = ft_stripAfterSemicolon(tokens[2]);
            else if (key == "return" && tokens.size() >= 3) 
            {
                currentRoute.redirectStatus = ft_stoi(ft_stripAfterSemicolon(tokens[1]));
                currentRoute.redirectPath = ft_stripAfterSemicolon(tokens[2]);
            }
            continue;
        }

        // ---------- Server-level Directives ----------
        std::string key = tokens[0];

        if (key == "listen")
            currentServer.port = ft_stoi(ft_stripAfterSemicolon(tokens[1]));
        else if (key == "host")
            currentServer.host = ft_stripAfterSemicolon(tokens[1]);
        else if (key == "server_name") 
        {
            for (size_t i = 1; i < tokens.size(); ++i) 
                currentServer.serverNames.push_back(ft_stripAfterSemicolon(tokens[i]));
        } 
        else if (key == "client_max_body_size") 
            currentServer.clientMaxBodySize = ft_stoi(ft_stripAfterSemicolon(tokens[1]));
        else if (key == "error_page" && tokens.size() >= 3) 
            currentServer.errorPages[ft_stoi(ft_stripAfterSemicolon(tokens[1]))] = ft_stripAfterSemicolon(tokens[2]);
    }

    file.close();
    return servers;
}


void ft_printServer(const std::map<int, std::vector<ServerConfig> >& serversByPort) 
{
    std::cout << "\nAll Ports open for service\n";
    for (std::map<int, std::vector<ServerConfig> >::const_iterator portIt = serversByPort.begin();
         portIt != serversByPort.end(); 
         ++portIt) 
    {
        int port = portIt->first;
        const std::vector<ServerConfig>& servers = portIt->second;

        std::cout << "=========================\n";
        std::cout << CYAN;
        std::cout << "Port: " << port << "\n";
        std::cout << CYAN << RESET;

        for (size_t i = 0; i < servers.size(); ++i) 
        {
            const ServerConfig& s = servers[i];

            std::cout << "Host: " << s.host << "\n";
            std::cout << "Client Max Body Size: " << s.clientMaxBodySize << "\n";

            std::cout << "Server Names:\n";
            for (size_t j = 0; j < s.serverNames.size(); ++j)
                std::cout << "  - " << s.serverNames[j] << "\n";

            std::cout << "Error Pages:\n";
            for (std::map<int, std::string>::const_iterator ep = s.errorPages.begin(); ep != s.errorPages.end(); ++ep)
                std::cout << "  " << ep->first << " -> " << ep->second << "\n";

            std::cout << "Routes:\n";
            for (size_t j = 0; j < s.routes.size(); ++j) 
            {
                const RouteConfig& r = s.routes[j];
                std::cout << "  Path: " << r.path << "\n";
                std::cout << "    Root: " << r.root << "\n";
                std::cout << "    Index: " << r.index << "\n";
                std::cout << "    Methods:";
                for (size_t m = 0; m < r.methods.size(); ++m)
                    std::cout << " " << r.methods[m];
                std::cout << "\n";


				std::cout << "    Client Max Body Size: " << r.clientMaxBodySize << "\n";
				std::cout << "    Upload Store: " << r.uploadStore << "\n";
				std::cout << "    Autoindex: " << (r.autoIndex ? "on" : "off") << "\n";

				

                if (!r.cgiExtensions.empty()) {
                    std::cout << "    CGI:\n";
                    for (std::map<std::string, std::string>::const_iterator cgi = r.cgiExtensions.begin(); cgi != r.cgiExtensions.end(); ++cgi)
                        std::cout << "      " << cgi->first << " -> " << cgi->second << "\n";
                }

                if (r.redirectStatus)
                    std::cout << "    Redirect: " << r.redirectStatus << " " << r.redirectPath << "\n";
            }
            std::cout << "-------------------------\n\n";
        }
    }
}


void ft_setNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


int ft_parseConfigAndSetEpoll(char **argv,
                         std::map<int, std::vector<ServerConfig> > &serversByPort,
                         std::map<int, int> &portToFd,
                         int &epoll_fd,
                         epoll_event *events) 
{
    (void)events;
    try 
    {
        serversByPort = ft_parseConfigFile(argv[1]);
        ft_printServer(serversByPort);
    } 
    catch (const std::exception &e) 
    {
        std::cerr << "Error parsing config: " << e.what() << "\n";
        return 1;
    }

    epoll_fd = epoll_create1(0);
    epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));

    for (std::map<int, std::vector<ServerConfig> >::iterator it = serversByPort.begin(); 
        it != serversByPort.end(); 
        ++it) 
    {
        int port = it->first;

        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(server_fd, (sockaddr *)&addr, sizeof(addr));
        listen(server_fd, 10);
        ft_setNonBlocking(server_fd);

        ev.events = EPOLLIN;
        ev.data.fd = server_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

        portToFd[port] = server_fd;
        std::cout << "Listening on port: " <<GREEN<< port <<RESET<< " (fd=" << server_fd << ")\n";
    }
    std::cout << "\n";

    return 0;
}

