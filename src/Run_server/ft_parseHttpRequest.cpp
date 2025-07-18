

#include "../../include/HttpRequest.hpp"

int HttpRequest::parseHttpRequest(char* buffer, HttpRequest& req) 
{
    req.rawBody.assign(buffer, buffer + strlen(buffer)); 
         
    std::string         request(buffer);
    std::istringstream  stream(request);
    std::string         line;
        
    if (std::getline(stream, line)) 
    {
        std::istringstream      reqLine(line);
        reqLine >> req.method >> req.path >> req.version >> req.afterVersion;
        size_t      qPos = req.path.find('?');
        if (qPos != std::string::npos) {
            req.query = req.path.substr(qPos + 1);
            req.path = req.path.substr(0, qPos);
        }
    }
    while (std::getline(stream, line) && line != "\r") {
        size_t      pos = line.find(':');
        if (pos != std::string::npos) {
            std::string     key = line.substr(0, pos);
            std::string     value = line.substr(pos + 1);

            key.erase(key.find_last_not_of(" \r\n") + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" \r\n") + 1);
            req.headers[key] = value;
            if (key == "Content-Length")
                req.contentLength = std::atoi(value.c_str());
            else if (key == "Content-Type")
                req.contenttype = value;
            else if (key == "Cookie")
                req.cookie = value;
        }
    }

    std::string     body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }

    if (!body.empty()) {
        req.body = body;
    }

    size_t  lastSlash = req.path.find_last_of('/');
    if (lastSlash != std::string::npos && lastSlash + 1 < req.path.length()) {
        req.filename = req.path.substr(lastSlash + 1);
    } else {
        req.filename = "index.html"; 
    }
    req.complete = true;

    // std::cout << GREEN;
    // printHttpRequest(req);
    // std::cout << GREEN << RESET;

    return(0);
}
  

void HttpRequest::printHttpRequest(const HttpRequest& req) 
{
    std::cout << "===== HttpRequest Detail =====\n";

    std::cout << std::left << std::setw(18) << "method:"         << req.method        << "\n";
    std::cout << std::left << std::setw(18) << "path:"           << req.path          << "\n";
    std::cout << std::left << std::setw(18) << "query:"          << req.query         << "\n";
    std::cout << std::left << std::setw(18) << "version:"        << req.version       << "\n";
    std::cout << std::left << std::setw(18) << "filename:"       << req.filename      << "\n";
    std::cout << std::left << std::setw(18) << "contentType:"    << req.contenttype   << "\n";
    std::cout << std::left << std::setw(18) << "cookie:"         << req.cookie        << "\n";
    std::cout << std::left << std::setw(18) << "contentLength:"  << req.contentLength << "\n";
    std::cout << std::left << std::setw(18) << "complete:"       << (req.complete ? "true" : "false") << "\n";

    std::cout << std::left << std::setw(18) << "headers:"        << "\n";
    for (std::map<std::string, std::string>::const_iterator it = req.headers.begin(); 
        it != req.headers.end(); 
        ++it) {
        std::cout << "  - " << it->first << ": " << it->second   << "\n";
    }

    std::cout << std::left << std::setw(18) << "body:" << "\n"   << req.body << "\n";

    std::cout << std::left << std::setw(18) << "rawBody size:"   << req.rawBody.size() << " bytes\n";
    std::cout << "===============================\n";
}



