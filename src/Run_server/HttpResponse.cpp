

#include "../../include/HttpResponse.hpp"

// HttpResponse::HttpResponse()    : status(200)   // ค่าเริ่มต้น (OK)
HttpResponse::HttpResponse()   
{
}

HttpResponse::~HttpResponse()
{
}





bool	HttpResponse::getStaticFile(std::string const &filePath )
{

	std::cout  << "getStaticFile 21\n";
	// Logger::log(LC_MINOR_NOTE, " in getStaticFile() , filePath = " , filePath.c_str());

	std::ifstream file(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{		
		struct stat fileStat;	

		// Logger::log(LC_MINOR_NOTE, " getStaticFile ==> filePath = %s" , filePath.c_str());
		if (stat(filePath.c_str(), &fileStat) != 0)
		{

			std::cout << ERROR_COLOR;
			std::cout  << "getStaticFile 58 Error if(stat(filePath.c_str(), &fileStat) != 0)\n";
			std::cout << ERROR_COLOR << RESET;

			if (errno == ENOENT)
				throw RequestException(404, "File not found");
			else if (errno == EACCES)
				throw RequestException(403, "Forbidden");
			else
				throw RequestException(405, "Method not allowed");
		}
		return false;
	}
	status = 200;

	size_t dotPos = filePath.find_last_of(".");
	std::string extension;
	if (dotPos != std::string::npos)
		extension = filePath.substr(dotPos);
	else
		extension = "";

	setHeader("Content-Type", getMimeType(extension), true);

	std::stringstream buffer;
	buffer << file.rdbuf();	 
	setBody(buffer.str());	


	std::cout  << "getStaticFile 114 (finish)\n";
	return true;  
	
}



bool HttpResponse::setHeader(std::string name, std::string value , bool overwriteExisting)
{
	std::map<std::string, std::string>::const_iterator pos = headers.find(name);
	if (pos != headers.end() && !overwriteExisting)
	{
		while (pos != headers.end())
		{
			name += " ";
			pos = headers.find(name);

		}
		headers[name] = value;
		return true;
	}
	else
	{
		headers[name] = value;
		return true;
	}
}


std::string HttpResponse::getMimeType(const std::string & extension)
{
	std::map<std::string, std::string> mimeTypes;
	mimeTypes[".html"] = "text/html";
    mimeTypes[".htm"] = "text/html";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".json"] = "application/json";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".gif"] = "image/gif";
    mimeTypes[".svg"] = "image/svg+xml";
    mimeTypes[".ico"] = "image/x-icon";
    mimeTypes[".xml"] = "application/xml";
    mimeTypes[".pdf"] = "application/pdf";
    mimeTypes[".txt"] = "text/plain";
	std::map<std::string, std::string>::const_iterator it = mimeTypes.find(extension);
	if (it != mimeTypes.end())
		return it->second;
	return "application/octet-stream";
}


bool	HttpResponse::setBody(std::string newBody)
{
 	body = newBody; 
	return true;
}




int HttpResponse::getStatus() const {
    return status;
}

std::map<std::string, std::string> HttpResponse::getHeaders() const {
    return headers;
}

std::string HttpResponse::getBody() const {
    return body;
}



void HttpResponse::printHeadersStatusBody() const {

	std::cout << RESPONSE_COLOR;
    std::cout << "======= HttpResponse::printHeadersStatusBody() 203 =====" << std::endl;
    std::cout << RESPONSE_COLOR << "Status	: " << RESET << status << std::endl;

    std::cout << RESPONSE_COLOR << "Headers	:" << RESET << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << RESPONSE_COLOR << "Body	:" <<  RESET << std::endl;
    std::cout << body << std::endl;
	std::cout << RESPONSE_COLOR;
    std::cout << "===================================" << std::endl;
	std::cout << RESPONSE_COLOR <<  RESET ;

}
