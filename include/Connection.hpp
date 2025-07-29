#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <ctime>
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
//#include "HttpResponse.hpp"
# include	<ctime>
# include 	<cstdio>
# include	<fcntl.h>
# include	<stdexcept>
# include	<string>
# include 	<errno.h>
# include 	<sys/epoll.h>
# include 	<sstream>
# include 	<unistd.h>


class Connection {
	private:
		int					socket;	
		//time_t				expiresOn; 
		ServerConfig		serverConfig;
		bool				isReady;
		int					bodyLength;
		std::string			boundary;
		std::string			requestBuffer;
		std::vector<char>	rawPostBody;
		std::string 		responseBuffer;		
		bool				headerIsCompleted;
		bool				requestIsCompleted;
		//int					epollSocket;
		size_t 				contentLength;


	public:
		Connection();
		Connection(int socket, ServerConfig config);

		void 				clear();

};

#endif
