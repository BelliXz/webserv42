
#include "Connection.hpp"

Connection::Connection()
{
	socket = 0;
	isReady = false;
	bodyLength = 0;
	contentLength = 0;
	
	//expiresOn = time(NULL) + (CON_SOC_TIMEOUT_SECS);
	rawPostBody.clear();

}

Connection::Connection(int socket, ServerConfig config):socket(socket), serverConfig(config),isReady(false)
{
	bodyLength = 0;
	contentLength = 0;
	
	//expiresOn = time(NULL) + (CON_SOC_TIMEOUT_SECS);
    int flags = fcntl(socket, F_GETFL, 0);
	if (flags >= 0) {
		if( fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
			throw std::runtime_error("Failed  to set socket to non-blocking mode");
	}
	rawPostBody.clear();
}

void Connection::clear()
{
	rawPostBody.clear();
	isReady = false;
	headerIsCompleted = false;
	requestIsCompleted = false;
	responseBuffer = "";
	contentLength = 0;
	bodyLength = 0;
	
}


void Connection::printConnection() const {
	std::cout << "======= Connection::printConnection() 46 ===========\n";
	std::cout << std::left << std::setw(25) << "Field" << "Value\n";
	std::cout << std::setw(25) << "Socket" << socket << "\n";
	std::cout << std::setw(25) << "isReady" << std::boolalpha << isReady << "\n";
	std::cout << std::setw(25) << "Body Length" << bodyLength << "\n";
	std::cout << std::setw(25) << "Boundary" << boundary << "\n";
	std::cout << std::setw(25) << "Request Buffer" << requestBuffer << "\n";
	std::cout << std::setw(25) << "Raw POST Body Size" << rawPostBody.size() << " bytes\n";
	std::cout << std::setw(25) << "Response Buffer" << responseBuffer << "\n";
	std::cout << std::setw(25) << "Header Completed" << std::boolalpha << headerIsCompleted << "\n";
	std::cout << std::setw(25) << "Request Completed" << std::boolalpha << requestIsCompleted << "\n";
	std::cout << std::setw(25) << "Content Length" << contentLength << "\n";
	std::cout << "==================================================\n";
}





ServerConfig* selectServerConfig(std::vector<ServerConfig> &servers, const HttpRequest &req) {

    (void)req;
    std::cout << "		ft selectServerConfig 107 req.port = " << SERVER_COLOR <<req.port << RESET<< "\n";
    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i].getPort() == req.port) {
            std::cout << "		ft selectServerConfig 111 เจอ port ที่ตรง\n" ;
            return &servers[i];
        }
    }
    if (!servers.empty())
    {
        std::cout <<"		ft selectServerConfig 120 ไม่เจอ port ที่ตรง\n";
        // return &servers[0];
    }
    return NULL;
}



RouteConfig* selectRouteConfig(ServerConfig &server, const std::string &path) 
{
    std::cout << "selectRouteConfig 134 request.path  : " << path << std::endl;
    // --- ตัด path ให้เหลือเฉพาะ directory ถ้ามี file (มี . หลัง /)
    std::string matchPath = path;
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash != std::string::npos) {
        std::string lastPart = path.substr(lastSlash + 1); // เช่น s2.txt
        if (lastPart.find('.') != std::string::npos) {
            matchPath = path.substr(0, lastSlash + 1); // เช่น /uploads/
        }
    }
    std::cout << "selectRouteConfig 144 normalized path for route compare: " << matchPath << std::endl;
    const std::map<std::string, RouteConfig> &routes = server.getRoutes();
    for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin();
         it != routes.end(); 
         ++it)
    {
        const std::string &routePath = it->first;
        // พิมพ์ทุกตัวที่กำลังเช็ค
        std::cout << "selectRouteConfig 152 check compare : " << routePath;
        if (routePath == matchPath) {
            std::cout << "      [MATCH]" << std::endl;

            // พิมพ์รายละเอียดของ route ที่ match
            std::cout << "selectRouteConfig 157 call printRouteConfig\n";
            RouteConfig::printRouteConfig(it->second);
            return const_cast<RouteConfig*>(&it->second);
        } else {
            std::cout << ERROR_COLOR << "      [not match]" << RESET <<  std::endl;
        }
    }
    std::cout << "selectRouteConfig 164 check compare : " 
              << ERROR_COLOR << "[route-check all] no route matched." 
              << RESET << std::endl;
    return NULL;
}



std::string resolvePath(const RouteConfig &route, const std::string &reqPath) 
{
    // 1. relativePath = reqPath - route.getPath()
    std::string relativePath = reqPath.substr(route.getPath().length());
    if (!relativePath.empty() && relativePath[0] != '/')
        relativePath = "/" + relativePath;

    // 2. ถ้า path เป็น directory → เติม index
    if (relativePath.empty() || relativePath[relativePath.length() - 1] == '/') {
        if (!route.getIndex().empty())
            relativePath += route.getIndex();
        else
            relativePath += "index.html";
    }

    // 3. ประกอบ root + relativePath
    std::string full;
    std::string root = route.getRoot();
    if (!root.empty() && root[root.size() - 1] == '/' && relativePath[0] == '/')
        full = "./" + root + relativePath.substr(1);
    else if (!root.empty() && root[root.size() - 1] != '/' && relativePath[0] != '/')
        full = "./" + root + "/" + relativePath;
    else
        full = "./" + root + relativePath;


    // // 4. ตรวจสอบว่าไฟล์มีจริง
    // struct stat st;
    // if (::stat(full.c_str(), &st) == 0) {
    //     return full;
    // }
    // return "";



    // 4. ไม่ !! ตรวจสอบว่าไฟล์มีจริง
    return full;
  
}



std::string ft_extractFileName(const std::string &path, bool containsDot) {
    size_t posSlash = path.find_last_of('/');
    std::string temp;

    if (posSlash == std::string::npos)
        temp = path;
    else
        temp = path.substr(posSlash + 1);

    if (temp.empty())
        return "";

    if (containsDot) {
        // ต้องมี . ถึงจะคืนค่า
        return (temp.find('.') != std::string::npos) ? temp : "";
    } else {
        // ตัดนามสกุลออก ถ้ามี .
        size_t posDot = temp.find_last_of('.');
        if (posDot != std::string::npos)
            return temp.substr(0, posDot);
        else
            return temp; // ไม่มี . ก็คืนชื่อไฟล์ตรง ๆ
    }
}


std::string ft_getFileExtension(const std::string &filePath)
{
    size_t dotPos = filePath.find_last_of(".");
    if (dotPos != std::string::npos)
        return filePath.substr(dotPos);  // คืนค่าพร้อมจุด เช่น ".py"
    return "";
}



bool Connection::appendRequestBufferAndProcess(
                                        char *buffer,
                                        HttpRequest &request,
                                        size_t length,
                                        std::vector<ServerConfig> &servers) 
{

    (void)buffer;
    (void)request;
    (void)length;
    (void)servers;

    std::cout << "======= Connection::appendRequestBufferAndProcess 295 ========";
    std::cout <<REQUEST_COLOR<< "\nbytes   = length  = "     << RESET  << length ;
    std::cout <<REQUEST_COLOR<< "\nrequest = buffer  = \n"  << RESET  << buffer << "\n";

    // --- 1) Append buffer ลง internal request buffer ---
    requestBuffer.append(buffer, length);
    // std::cout <<RED <<  "\nrequestBuffer = " <<  requestBuffer << "\n" << RESET;

    // --- 2) หา header end ---
    size_t headerEndPos = requestBuffer.find("\r\n\r\n");
    std::cout  <<  "\n		append 179 " << REQUEST_COLOR "headerEndPos = requestBuffer.find(r n r n); =" << headerEndPos  << "\n" << RESET;
    if (headerEndPos == std::string::npos) {
        // Case #1: ยังไม่เจอ \r\n\r\n
        std::cout  <<  "		append 182 errorheaderEndPos == std::string::npos";
        return false;
    }

    // *** จุดนี้ "request" ที่ส่งเข้ามาคือ parse แล้วจากข้างนอก ***
    // เช่นใน caller:
    // HttpRequest req;
    // req.parseHttpRequest(buffer, req);
    // appendRequestBufferAndProcess(buffer, req, length, servers);

    // --- 3) เช็ก body ---
    size_t bodyStartPos = headerEndPos + 4;
    size_t bodyBytes = requestBuffer.size() - bodyStartPos;

    // กำหนด contentLength ถ้า method POST
    if (request.method == "POST") {
        if (request.headers.find("Content-Length") == request.headers.end()) {
            std::cout << "append 180 if (request.headers.find(Content-Length) == request.headers.end())\n";
            throw RequestException(411, "Length Required");
        }
        request.contentLength = std::atoi(request.headers["Content-Length"].c_str());
    } else {
        request.contentLength = 0;
    }

    if (bodyBytes < request.contentLength) {
         std::cout << "append 189 bodyBytes < request.contentLength ";
        // Case #2: header ครบ + body ยังไม่ครบ
        return false;
    }
    // Case #3, #4, #5 → ผ่าน

    // --- 4) เช็ก error ตามที่กำหนด ---
    // 1. Path มี ".."
    if (request.path.find("..") != std::string::npos) {
        std::cout  << "append 198 find ..";
        throw RequestException(400, "Bad Request");
    }



    // // หา server config ตาม host/port
    srvConf = selectServerConfig(servers, request);
    std::cout  <<MONITOR_COLOR <<  "==== re-check  append 261 srvConf   = selectServerConfig(servers, request) ====\n"  << RESET;
    // ConfigParser::printSelectedServer(srvConf);//<==print
    if (!srvConf) {
        std::cout  << "append 264 if (!srvConf)";
        throw RequestException(403, "Forbidden");
    }


    // หา route config ตาม path
    std::cout  << MONITOR_COLOR << "==== re-check  append 271 routeConf = selectRouteConfig(*srvConf, request.path) ===="<< RESET;
    std::cout  << "\n";
    routeConf = selectRouteConfig(*srvConf, request.path);
    if (!routeConf) {
        std::cout << "append 276 if (!routeConf)";
        throw RequestException(403, "Forbidden");
    }


    std::cout  << MONITOR_COLOR << "==== re-check  append 281 resolvedPath = resolvePath(*routeConf, request.path); ===="<< RESET;
    std::cout  << "\n";
    // 6. Resolve path ไม่ได้
    resolvedPath = resolvePath(*routeConf, request.path);
    // if (resolvedPath.empty()) {
    //     std::cout << "308 if (resolvedPath.empty())";
    //     throw RequestException(403, "Forbidden");
    // }
    std::cout  << "append 342 resolvedPath         = " << SERVER_COLOR << resolvedPath << RESET << "\n";

    
    // std::string path = "/var/www/html/index.html";
    std::string fileWithExt = ft_extractFileName(resolvedPath, true);   // "sessionTest.py"
    std::string fileNoExt   = ft_extractFileName(resolvedPath, false);  // "sessionTest"
    std::string ext         = ft_getFileExtension(fileWithExt);         // ".py"
    std::string cmd         = routeConf->getCGI(ext);                   // /usr/bin/python3
    // std::string cmd         = routeConf->getCGI(".py");



    bool		isUploadRequest = false;
    if(!routeConf->getUploadStore().empty())
    {
        // the request URL must be exactly match to the route path
        if(request.path == routeConf->getPath() && request.method == "POST")
            isUploadRequest = true; 
    }


    bool allowDirectoryListing = false;
    allowDirectoryListing = routeConf->getAutoindex();

    // std::cout  << "append 366\n";
    std::cout << "append 422 port 	        = " 		  << SERVER_COLOR << serverConfig.getPort() << RESET << std::endl;
    std::cout << "append 422 requestBuffer        = \n" << SERVER_COLOR	<< requestBuffer << RESET << std::endl;
    std::cout << "append 422 resolvedPath         = "     << SERVER_COLOR << resolvedPath << RESET<< std::endl;
    std::cout << "append 422 fileWithExt	        = "     << fileWithExt << std::endl;
    std::cout << "append 422 fileNoExt	        = "         << fileNoExt << std::endl;
    std::cout << "append 422 ext	                = "     << ext << std::endl;
    std::cout << "append 422 cmd 		        = " 			<< cmd << std::endl;
    std::cout << "append 422 isUploadRequest 	= " 		<< (isUploadRequest ? "true" : "false") << std::endl;
    std::cout << "append 422 allowDiretoryBrowsing= " << (allowDirectoryListing ? "true" : "false") << std::endl;
    std::cout << "\n\n\n";
    ////////////////////////////////////////////////////////////////////////////////////






    // // 2. ถ้ามี directive return → redirect
    // if (routeConf->getReturnStatus() >= 300 && routeConf->getReturnStatus() < 400) {
    //     std::cout <<"284 if (routeConf->getReturnStatus() >= 300 && routeConf->getReturnStatus() < 400)";
    //     throw RequestException(routeConf->getReturnStatus(), routeConf->getReturnValue());
    // }

    // // 3. Method ไม่ถูกต้อง
    // const std::vector<std::string> &allow = routeConf->getMethods();
    // if (std::find(allow.begin(), allow.end(), request.method) == allow.end()) {
    //     std::cout << "291 if (std::find(allow.begin(), allow.end(), request.method) == allow.end())";
    //     throw RequestException(405, "Method Not Allowed");
    // }

    // // 4. POST ไม่มี Content-Length → ข้างบนเช็กแล้ว

    // // 5. Body ใหญ่เกิน limit
    // size_t maxBodySize = routeConf->getClientMaxBodySize() > 0 ?
    //                      routeConf->getClientMaxBodySize() : srvConf->getClientMaxBodySize();
    // if (bodyBytes > maxBodySize) {
    //     std::cout << "301 if (bodyBytes > maxBodySize)";
    //     std::cout << "\nbodyBytes =" << bodyBytes ;
    //     std::cout << "\nmaxBodySize =" << maxBodySize ;
    //     throw RequestException(413, "Payload Too Large");
    // }



    // // ////////////////////////////////////////////////////////////////////////////////////
    // // std::cout  << MONITOR_COLOR << "==== re-check  append 281 resolvedPath = resolvePath(*routeConf, request.path); ===="<< RESET;
    // // std::cout  << "\n";
    // // // 6. Resolve path ไม่ได้
    // // // std::string resolvedPath = resolvePath(*routeConf, request.path);// chck
    // // resolvedPath = resolvePath(*routeConf, request.path);
    // // if (resolvedPath.empty()) {
    // //     std::cout << "308 if (resolvedPath.empty())";
    // //     throw RequestException(403, "Forbidden");
    // // }
    

    // std::cout  << "append 274 resolvedPath = " << REQUEST_COLOR << resolvedPath << RESET <<"\n";

    // // --- 5) ครบแล้ว mark complete ---
    // request.body = requestBuffer.substr(bodyStartPos, request.contentLength);
    // request.complete = true;
    // std::cout << "append 291 finish\n\n\n";

    return true; // ครบพร้อมประมวลผล
}




bool Connection::handleHttpResponse(HttpRequest &request,
    std::vector<ServerConfig> &servers,
    size_t length)
{
    std::cout  <<MONITOR_COLOR << "Response 507 \n" << RESET;
    (void)length;
    (void)servers;

    try {
        std::string localPath = getResolvedPath();
        std::cout  << "Response 529 localPath = " <<REQUEST_COLOR << localPath << RESET<< "\n";

        bool isUploadRequest = false;           // TODO: ตรวจจาก request
        std::string cmd;                        // TODO: ถ้าเป็น CGI ใส่ command
        std::string rawPostBody;                // TODO: ดึงจาก request body
        std::string requestPathContainFile = localPath;     // TODO: ตรวจว่า path เป็นไฟล์จริง
        bool allowDirectoryBrowsing = false;    // TODO: ดึงจาก route config

        HttpResponse httpResponse;

        if (request.method == "DELETE") {
            httpResponse.handleDeleteMethod(localPath);
        }
        else if (isUploadRequest) {
            // httpResponse.handleUploadedFiles(this, route, request);
        }
        else if (!cmd.empty()) {
            // httpResponse.processCGI(cmd, localPath, request, *srvConf, *route, rawPostBody);
        }
        else if (!requestPathContainFile.empty()) {

            std::cout  <<MONITOR_COLOR << "Response 555 if(!requestPathContainFile.empty())\n" <<RESET;

            httpResponse.getStaticFile(localPath);   
            std::cout << "\n\nResponse 558 call httpResponse.printHeadersStatusBody()\n"; 
            httpResponse.printHeadersStatusBody();

        }
        else if (allowDirectoryBrowsing) {
            // httpResponse.generateDirectoryListing(request, localPath);
        }
        else {
            std::cout  << "no DELETE  UPLOAD cmd.empty allow\n";
            throw RequestException(403, "Forbidden");
        }

        // ส่ง response กลับ client
        // ready(httpResponse, true);


        // conn สร้างไว้แล้ว
        std::cout << MONITOR_COLOR ;
        std::cout << "\n\nResponse 579 call prepareResponse +sendResponse\n"; 
        std::cout << MONITOR_COLOR << RESET ;
        prepareResponse(httpResponse);
        // sendResponse();   // send at ConnectionManager::write




        return true;
    }
    catch (const RequestException &e) {
        // std::cout << 

        std::cout << ERROR_COLOR;
        std::cout  << "Response 590 catch e.what = ";
        std::cerr << e.what() <<"\n"  ;
		std::cout << ERROR_COLOR << RESET;




        HttpResponse errorResp;//????
        
        // errorResp.setStatus(e.getStatusCode());
        // errorResp.setBody(HttpResponse::getErrorPage(e.getStatusCode(), getServerConfig()));
        // ready(errorResp, true);
    }
    return false;
}


