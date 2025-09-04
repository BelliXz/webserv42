#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ConfigParser.hpp"


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


		// use appendRequestBufferAndProcess  + handleHttpResponse
		ServerConfig 		*srvConf;
		RouteConfig 		*routeConf;
		std::string 		resolvedPath;




	public:
		Connection();
		Connection(int socket, ServerConfig config);

		void 				clear();


		std::string 		getResolvedPath(){
			return (resolvedPath);
		}

		std::string 		getRequestBuffer() const{
			return (requestBuffer);
		}

		int 				getSocket() const{
			return (socket);
		}
		int 				getBodyLength() const{
			return (bodyLength);
		}



		ServerConfig		&getServerConfig(){
			return serverConfig;
		}		//////////////////////////////////////


		std::string 		getResponseBuffer() const{
			return (responseBuffer);
		}


		
	    void prepareResponse(const HttpResponse &response) {
	        responseBuffer = response.ResponseToString();
	    }



	    void sendResponse() {
	        if (!responseBuffer.empty()) {
	            send(socket,
	                 responseBuffer.c_str(),
	                 responseBuffer.size(),
	                 MSG_DONTWAIT);
	        }
	    }


		void printConnection() const ;

		bool appendRequestBufferAndProcess(
			char *buffer,
			HttpRequest &request,
			size_t length,
			std::vector<ServerConfig> &servers
		);

		bool handleHttpResponse(HttpRequest &request,
			std::vector<ServerConfig> &servers,
			size_t length);

		void ready(const HttpResponse &response, bool closeAfterSend);

		// ServerConfig &getServerConfig();
};

#endif
