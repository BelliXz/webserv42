
#include "../include/ConfigParser.hpp"

std::vector<ServerConfig> ConfigParser::parseAllConfigs(const std::string& configFilePath)
{
	(void)configFilePath;
	std::vector<ServerConfig> servers;

	// -------- Server [0] --------
	ServerConfig s0;
	s0.setPort(8080);
	s0.setServerName("localhost");
	s0.setHost("127.0.0.1");
	s0.setClientMaxBodySize(1000);
	s0.addErrorPage(404, "errors/404.html");
	s0.addErrorPage(500, "errors/500.html");

	{
		RouteConfig r;
		r.setPath("/");
		r.setRoot("www/html;<=asdfghnm");
		r.setIndex("index.html");
		r.setClientMaxBodySize(1000);
	
		std::vector<std::string> methods;
		methods.push_back("POST");
		methods.push_back("DELETE");
		methods.push_back("GET");
		r.setMethods(methods);

		// autoindex off (default false)
		s0.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/uploads");
		r.setRoot("www/html/uploads;<====");
		
		std::vector<std::string> methods;
		methods.push_back("GET");
		methods.push_back("POST");
		methods.push_back("DELETE");
		r.setMethods(methods);

		r.setClientMaxBodySize(800);
		r.setUploadStore("www/html/uploads;<=======4");
		r.setAutoindex(true);
		s0.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/cgi-bin");
		r.setRoot("www/html/cgi-bin;<=qwerfghj");
		r.setIndex("setName.py");

		std::vector<std::string> methods;
		methods.push_back("GET");
		methods.push_back("POST");
		r.setMethods(methods);

		r.addCGI(".php", "/usr/bin/php-cgi");
		r.addCGI(".py", "/usr/bin/python3");
		s0.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/cookie");
		r.setRoot("");
		r.setUploadStore("cookie/session;<====");
		r.setIndex("session.py");

		std::vector<std::string> methods;
		methods.push_back("GET");
		methods.push_back("POST");
		r.setMethods(methods);

		r.addCGI(".php", "/usr/bin/php-cgi");
		r.addCGI(".py", "/usr/bin/python3");
		s0.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/redirect");
		r.setReturn(301, "/");
		s0.addRoute(r.getPath(), r);
	}
	servers.push_back(s0);

	// -------- Server [1] --------
	ServerConfig s1;
	s1.setPort(9091);
	s1.setServerName("test.local");
	s1.setHost("127.0.0.1");
	s1.setClientMaxBodySize(15);

	{
		RouteConfig r;
		r.setPath("/");
		r.setRoot("www/test");
		r.setIndex("test.html");

		std::vector<std::string> methods;
		methods.push_back("GET");
		r.setMethods(methods);

		s1.addRoute(r.getPath(), r);
	}
	servers.push_back(s1);

	// -------- Server [2] --------
	ServerConfig s2;
	s2.setPort(8888);
	s2.setServerName("test_php");
	s2.setHost("127.0.0.1");
	s2.setClientMaxBodySize(15);

	{
		RouteConfig r;
		r.setPath("/");
		r.setRoot("www/test");
		r.setIndex("test.php");

		std::vector<std::string> methods;
		methods.push_back("GET");
		methods.push_back("POST");
		methods.push_back("DELETE");
		r.setMethods(methods);

		r.addCGI(".php", "/usr/bin/php-cgi");
		r.addCGI(".py", "/usr/bin/python3");
		s2.addRoute(r.getPath(), r);
	}
	servers.push_back(s2);

	// -------- Server [3] --------
	ServerConfig s3;
	s3.setPort(9999);
	s3.setServerName("test9999");
	s3.setHost("127.0.0.1");

	{
		RouteConfig r;
		r.setPath("/");
		r.setRoot("www/html");
		r.setIndex("index.html");
		s3.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/upload");
		r.setRoot("uploads");


		std::vector<std::string> methods;
		methods.push_back("POST");
		r.setMethods(methods);

		r.setClientMaxBodySize(1000);
		s3.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("/old-path");
		r.setReturn(301, "/new-path");
		s3.addRoute(r.getPath(), r);
	}
	{
		RouteConfig r;
		r.setPath("~"); // for regex
		r.addCGI(".php", "/usr/bin/php-cgi");
		s3.addRoute(r.getPath(), r);
	}
	servers.push_back(s3);

	return servers;
}


bool ConfigParser::checkErrorParseAllConfigs(const std::vector<ServerConfig>& servers) 
{
	(void)servers;
	if (0)
		return true; // มี error
	else
		return false; // ไม่มี error
}



void ConfigParser::printConfigParser(const std::vector<ServerConfig>& servers) 
{
	for (size_t i = 0; i < servers.size(); ++i) 
	{
		const ServerConfig& srv = servers[i];
		std::cout << "---- Server [" << i << "] --------\n";
		std::cout << GREEN;
		std::cout << "Port: " << srv.getPort() << " =============\n";
		std::cout << GREEN << RESET;
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
			std::cout << GREEN;
			std::cout << "  Path: " << route.getPath() << "\n";
			std::cout << GREEN << RESET;
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
		std::cout << "\n";
	}
}
