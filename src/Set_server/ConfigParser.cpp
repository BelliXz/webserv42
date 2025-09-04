
#include "../include/ConfigParser.hpp"


static std::string trim(const std::string& s) 
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

static std::vector<std::string> split(const std::string& line, char delim = ' ') 
{
    std::vector<std::string>    tokens;
    std::stringstream           ss(line);
    std::string                 token;
    while (std::getline(ss, token, delim)) 
    {
        if (!token.empty())
            tokens.push_back(trim(token));
    }
    return tokens;
}

std::vector<ServerConfig> ConfigParser::parseAllConfigs(const std::string& configFilePath) 
{
    std::ifstream   file(configFilePath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error opening config file: " << configFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<ServerConfig>   servers;
    std::string                 line;
    ServerConfig                currentServer;
    RouteConfig                 currentRoute;
    bool                        inServer = false;
    bool                        inLocation = false;

    while (std::getline(file, line)) 
    {
        line = trim(line);
        if (line.empty() || line[0] == '#') 
            continue;

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
                currentServer.addRoute(currentRoute.getPath(), currentRoute);
                inLocation = false;
            } 
            else if (inServer) 
            {
                servers.push_back(currentServer);
                inServer = false;
            }
            continue;
        }

        if (!inServer) 
            continue;

        if (line.substr(0, 8) == "location") 
        {
            std::vector<std::string> tokens = split(line);
            if (tokens.size() >= 2) 
            {
                currentRoute = RouteConfig();
                currentRoute.setPath(tokens[1]);
                inLocation = true;
            }
            continue;
        }

        std::vector<std::string> tokens = split(line);
        if (tokens.empty()) 
            continue;

        std::string key = tokens[0];
        std::string value = line.substr(line.find(key) + key.length());
        value = trim(value);
        if (!value.empty() && value[value.size() - 1] == ';')
            value = value.substr(0, value.size() - 1);
        value = trim(value);

        if (!inLocation) 
        {
            if      (key == "listen")       currentServer.setPort(atoi(value.c_str()));
            else if (key == "server_name")  currentServer.setServerName(value);
            else if (key == "host")         currentServer.setHost(value);
            else if (key == "root")         currentServer.setRoot(value);
            else if (key == "index")        currentServer.setIndex(value);
            else if (key == "client_max_body_size") 
            {
                std::istringstream iss(value);
                size_t val; iss >> val;
                currentServer.setClientMaxBodySize(val);
            }
            else if (key == "error_page") 
            {
                std::vector<std::string> parts = split(value);
                if (parts.size() == 2)
                    currentServer.addErrorPage(atoi(parts[0].c_str()), parts[1]);
            }
        } 
        else 
        {
            if      (key == "root")     currentRoute.setRoot(value);
            else if (key == "index")    currentRoute.setIndex(value);
            else if (key == "allowed_methods" || key == "methods") currentRoute.setMethods(split(value));
            else if (key == "client_max_body_size") 
            {
                std::istringstream iss(value);
                size_t val; iss >> val;
                currentRoute.setClientMaxBodySize(val);
            }
            else if (key == "upload_store") currentRoute.setUploadStore(value);
            else if (key == "autoindex")    currentRoute.setAutoindex(value == "on");
            else if (key == "return") 
            {
                std::vector<std::string> parts = split(value);
                if (parts.size() == 2)
                    currentRoute.setReturn(atoi(parts[0].c_str()), parts[1]);
            }
            else if (key == "cgi_pass" && tokens.size() >= 3) 
            {
                currentRoute.addCGI(tokens[1], tokens[2]);
            }
        }
    }

    file.close();
    return servers;
}

bool ConfigParser::checkErrorParseAllConfigs(const std::vector<ServerConfig>& servers) 
{
    // pair< pair<host, port>, server_name >
    std::set< std::pair< std::pair<std::string, int>, std::string > > hostPortNameSet;

    for (size_t i = 0; i < servers.size(); ++i) 
    {
        const ServerConfig& server = servers[i];
        const std::string& host = server.getHost();
        int port = server.getPort();
        const std::string& name = server.getServerName();

        if (host.empty() || port == 0) 
        {   
            std::cerr << "Error: Missing host or port in server block #" << i << std::endl;
            return true;
        }

        // ทำ key = ((host, port), name)
        std::pair< std::pair<std::string, int>, std::string > key(
            std::make_pair(host, port),
            name
        );

        if (!hostPortNameSet.insert(key).second) {
            std::cerr << "Error: Duplicate server block #" << i
                      << " → host=" << host
                      << " port=" << port
                      << " server_name=" << name
                      << std::endl;
            return true;
        }
    }

    return false; // ✅ ไม่มี error
}




void ConfigParser::printAllServerConfig(const std::vector<ServerConfig>& servers) 
{
	std::cout << SERVER_COLOR;
	std::cout << "\n\n---- ConfigParser::printAllServerConfig --------\n";
	std::cout << SERVER_COLOR << RESET;
	for (size_t i = 0; i < servers.size(); ++i) 
	{
		const ServerConfig& srv = servers[i];
		std::cout << "---- Server [" << i << "] --------\n";
		std::cout << SERVER_COLOR;
		std::cout << "Port: " << srv.getPort() << " =============\n";
		std::cout << SERVER_COLOR << RESET;
		std::cout << "Host: " << srv.getHost() << "\n";
		std::cout << "Server Name: " << srv.getServerName() << "\n";
		std::cout << "Root: " << srv.getRoot() << "\n";
		std::cout << "Index: " << srv.getIndex() << "\n";
		std::cout << "Client Max Body Size: " << srv.getClientMaxBodySize() << "\n";
		std::cout << "Error Pages:\n";
		const std::map<int, std::string>& errPages = srv.getErrorPages();
		for (std::map<int, std::string>::const_iterator it = errPages.begin(); it != errPages.end(); ++it)
			std::cout << "  " << it->first << " => " << it->second << "\n";
		std::cout << "Routes:\n";
		const std::map<std::string, RouteConfig>& routes = srv.getRoutes();
		for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it) 
		{
			const RouteConfig& route = it->second;
			std::cout << SERVER_COLOR;
			std::cout << "  Path: " << route.getPath() << "\n";
			std::cout << SERVER_COLOR << RESET;
			std::cout << "    Root: " << route.getRoot() << "\n";
			std::cout << "    Index: " << route.getIndex() << "\n";
			std::cout << "    Autoindex: " << (route.getAutoindex() ? "on" : "off") << "\n";
			std::cout << "    Client Max Body Size: " << route.getClientMaxBodySize() << "\n";
			std::cout << "    Upload Store: " << route.getUploadStore() << "\n";
			// Allowed Methods
			std::cout << "    Allow Methods: ";
			const std::vector<std::string>& methods = route.getMethods();
			for (size_t j = 0; j < methods.size(); ++j) 
			{
				std::cout << methods[j];
				if (j + 1 < methods.size()) std::cout << ", ";
			}
			std::cout << "\n";
			// CGI Map
			std::cout << "    CGI Passes:\n";
			const std::map<std::string, std::string>& cgis = route.getCGIs();
			for (std::map<std::string, std::string>::const_iterator cit = cgis.begin(); cit != cgis.end(); ++cit) 
			{
				std::cout << "      " << cit->first << " => " << cit->second << "\n";
			}
			// Return Directive
			if (route.getReturnStatus() != 0) 
			{
				std::cout << "    Return: " << route.getReturnStatus() << " => " << route.getReturnValue() << "\n";
			}
		} 
		std::cout << "-----------------------------\n";
		std::cout << "\n";
	}
}




void ConfigParser::printSelectedServer(const ServerConfig* s)
{
	std::cout << "======= ConfigParser::printSelectedServer() 334 ===========\n";
	if (!s)
	{
		std::cout << RED << "Selected Server is NULL" << RESET << std::endl;
		return;
	}
	std::cout << SERVER_COLOR;
	std::cout << "---- ConfigParser::printSelectedServer --------\n";
	std::cout << SERVER_COLOR << RESET;
	std::cout << SERVER_COLOR;
	std::cout << "Port: " << s->getPort() << "\n";
	std::cout << SERVER_COLOR << RESET;
	std::cout << "Host: " << s->getHost() << "\n";
	std::cout << "Server Name: " << s->getServerName() << "\n";
	std::cout << "Root: " << s->getRoot() << "\n";
	std::cout << "Index: " << s->getIndex() << "\n";
	std::cout << "Client Max Body Size: " << s->getClientMaxBodySize() << "\n";
	std::cout << "Error Pages:\n";
	const std::map<int, std::string>& errPages = s->getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errPages.begin(); it != errPages.end(); ++it)
		std::cout << "  " << it->first << " => " << it->second << "\n";
	std::cout << "Routes:\n";
	const std::map<std::string, RouteConfig>& routes = s->getRoutes();
	for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it) 
	{
		const RouteConfig& route = it->second;
		std::cout << SERVER_COLOR << "  Path: " << route.getPath() << SERVER_COLOR << RESET << "\n";
		std::cout << "    Root: " << route.getRoot() << "\n";
		std::cout << "    Index: " << route.getIndex() << "\n";
		std::cout << "    Autoindex: " << (route.getAutoindex() ? "on" : "off") << "\n";
		std::cout << "    Client Max Body Size: " << route.getClientMaxBodySize() << "\n";
		std::cout << "    Upload Store: " << route.getUploadStore() << "\n";
		// Allowed Methods
		std::cout << "    Allow Methods: ";
		const std::vector<std::string>& methods = route.getMethods();
		for (size_t j = 0; j < methods.size(); ++j) 
		{
			std::cout << methods[j];
			if (j + 1 < methods.size()) std::cout << ", ";
		}
		std::cout << "\n";
		// CGI Map
		std::cout << "    CGI Passes:\n";
		const std::map<std::string, std::string>& cgis = route.getCGIs();
		for (std::map<std::string, std::string>::const_iterator cit = cgis.begin(); cit != cgis.end(); ++cit) 
		{
			std::cout << "      " << cit->first << " => " << cit->second << "\n";
		}
		// Return Directive
		if (route.getReturnStatus() != 0) 
		{
			std::cout << "    Return: " << route.getReturnStatus() << " => " << route.getReturnValue() << "\n";
		}
	}
	std::cout << "-----------------------------\n";
	std::cout << "\n";
}

