


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


	bool	getStaticFile(std::string const &filePath );



	bool 								setHeader(std::string name, std::string value , bool overwriteExisting=false);
	// // std::map<std::string, std::string> 	&getHeaders(void);

	bool								setStatus(int statusCode)
	{
		status = statusCode;
		return true;
	}

	// int									getStatus();

	// std::string							getBody() const;
	bool								setBody(std::string body);

	// std::string 						serialize();
	// void								clear();
	
	// static std::string					getStatusText(int statusCode);
	// static std::string					getErrorPage(int statusCode, ServerConfig server);
	// static std::string					getDefaultErrorPage(int statusCode);
	static std::string					getMimeType(const std::string & extension);
	
	
	
	
	int 								getStatus() const;
    std::map<std::string, std::string> 	getHeaders() const;
    std::string 						getBody() const;
	void 								printHeadersStatusBody() const;




	/////////////////////////////////////
	std::string getStatusMessage(int code) const {
        switch (code) {
            case 200: return "OK";
            case 404: return "Not Found";
            case 500: return "Internal Server Error";
            default:  return "Unknown";
        }
    }


	// build เป็น HTTP response string
	std::string ResponseToString() const 
	{
        std::ostringstream oss;

        // 1. Status line
        oss << "HTTP/1.1 " << status << " "
            << getStatusMessage(status) << "\r\n";

        // 2. Headers
        for (std::map<std::string, std::string>::const_iterator it = headers.begin();
             it != headers.end(); ++it) {
            oss << it->first << ": " << it->second << "\r\n";
        }

        // 3. Blank line
        oss << "\r\n";

        // 4. Body
        oss << body;

        return oss.str();
    }
	//////////////////////////////////////



	bool	handleDeleteMethod(std::string &localPath)
	{
	
		// สรุป logic:
		// log → ว่าจะลบไฟล์อะไร
		// เช็คว่าไฟล์นั้นมีอยู่และอนุญาตให้ลบ
		// ลบไฟล์ → ถ้าล้มเหลว โยน error 403
		// ถ้าลบสำเร็จ → ตั้ง status เป็น 204 และ body ว่าง
		
		// สถานการณ์ที่อาจทำให้ลบไม่สำเร็จ:
		// สาเหตุ					ผลลัพธ์
		// ไฟล์ไม่มี				403 Forbidden
		// ไม่มีสิทธิ์ลบไฟล์			403 Forbidden
		// ไฟล์ถูกใช้โดยโปรเซสอื่น		403 Forbidden
		// เป็นไดเรกทอรี (ไม่ใช่ไฟล์)	ขึ้นอยู่กับ checkFileAvailibity()
	
		if(!checkFileAvailibity(localPath, true))
		{
			std::cout << "116 handleDeleteMethod in handleDeleteMethod checkFileAvailibity false\n";
			throw RequestException(403, "Forbidden");	
		}
		if (remove(localPath.c_str()) != 0)
		{
			std::cout << "121 handleDeleteMethod in handleDeleteMethod remove false\n";
			throw RequestException(403, "Forbidden");
		}
		std::cout << "126 handleDeleteMethod in handleDeleteMethod remove success\n";
		setStatus(204);
		setBody("");
		std::cout << "127 handleDeleteMethod (finish)\n";

		return (true);
	}
	




	bool checkFileAvailibity(std::string &filePath, bool isFileOnly)
	{
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) != 0)
		{
			if (errno == ENOENT)
				throw RequestException(404, "142 checkFileAvailibity_throw File not found");
			else if (errno == EACCES)
				throw RequestException(403, "144 checkFileAvailibity_throwForbidden");
			else
				throw RequestException(400, "146 checkFileAvailibity_throwBad Request");
		}
	
		if (isFileOnly)
			return S_ISREG(fileStat.st_mode);
		return true;
	}

};




#endif // REQUESTEXCEPTION_HPP
