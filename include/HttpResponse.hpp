
#ifndef HTTPRESPOMSE_HPP
#define HTTPRESPOMSE_HPP


#include "RouteConfig.hpp"

class HttpResponse
{
	private:
		int										status;
		std::map<std::string, std::string>		headers;
		std::string								body;


	public:
		HttpResponse(/* args */);
		~HttpResponse();



		int 								getStatus() const;
	    std::map<std::string, std::string> 	getHeaders() const;
	    std::string 						getBody() const;


		bool								setStatus(int statusCode);
		bool 								setHeader(std::string name, std::string value , bool overwriteExisting=false);
		bool								setBody(std::string body);


		bool								getStaticFile(std::string const &filePath );
		static std::string					getMimeType(const std::string & extension);
		

		void 								printHeadersStatusBody() const;


		std::string 						getStatusMessage(int code) const ;
		std::string 						ResponseToString() const ;
		bool								handleDeleteMethod(std::string &localPath);
		bool 								checkFileAvailibity(std::string &filePath, bool isFileOnly);

};


#endif
