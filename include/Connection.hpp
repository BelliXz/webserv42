#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class Connection {
	private:
		int					socket;	
		//time_t			expiresOn; 
		ServerConfig		serverConfig;

		bool				isReady;
		int					bodyLength;
		std::string			boundary;
		std::string			requestBuffer;
		std::vector<char>	rawPostBody;
		std::string 		responseBuffer;		
		bool				headerIsCompleted;
		bool				requestIsCompleted;
		//int				epollSocket;
		size_t 				contentLength;


		// use appendRequestBufferAndProcess  + handleHttpResponse
		ServerConfig 		*srvConf;
		RouteConfig 		*routeConf;
		std::string 		resolvedPath;




	public:
		Connection();
		Connection(int socket, ServerConfig config);

		void 				clear();

		std::string 		getResolvedPath();
		std::string 		getRequestBuffer() const;
		int 				getSocket() const;
		int 				getBodyLength() const;
		ServerConfig		&getServerConfig();
		std::string 		getResponseBuffer() const;


		



		// Helper functions for  appendRequestBufferAndProcess
		// static ServerConfig* selectServerConfig(std::vector<ServerConfig> &servers, const HttpRequest &req) 
		// static RouteConfig* 	selectRouteConfig(ServerConfig &server, const std::string &path) 
		// static std::string 	resolvePath(const RouteConfig &route, const std::string &reqPath) 
		// static std::string 	extractFileName(const std::string &path, bool containsDot)
		// static std::string 	getFileExtension(const std::string &filePath)

		bool appendRequestBufferAndProcess(char *buffer,HttpRequest &request,size_t length,std::vector<ServerConfig> &servers);
		
		bool handleHttpResponse(HttpRequest &request,std::vector<ServerConfig> &servers,size_t length);
		void ready(const HttpResponse &response, bool closeAfterSend);
		
		void printConnection() const ;




	    void prepareResponse(const HttpResponse &response);	// รับ HttpResponse แล้วแปลงเป็น responseBuffer
	    void sendResponse() ;							 	// ส่งออกไป
};

#endif
