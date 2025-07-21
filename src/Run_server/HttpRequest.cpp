

#include "../../include/HttpRequest.hpp"

HttpRequest::HttpRequest():  contentLength(0) ,complete(false)
{
}

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string>    tokens;
    std::stringstream           ss(s);
    std::string                 item;
    while (std::getline(ss, item, delim)) 
    {
        tokens.push_back(item);
    }
    return tokens;
}

int HttpRequest::parseHttpRequest(char* buffer, HttpRequest& req) 
{
    req.rawBody.assign(buffer, buffer + strlen(buffer)); 
         
    std::string         request(buffer);
    std::istringstream  stream(request);
    std::string         line;

    req.request = request;
    if (std::getline(stream, line)) 
    {
        std::istringstream      reqLine(line);
        reqLine >> req.method >> req.rawPath >> req.version >> req.afterVersion;
        size_t                  qPos = req.rawPath.find('?');
        if (qPos != std::string::npos) 
        {
            req.rawQueryString = req.rawPath.substr(qPos + 1);
            req.path           = req.rawPath.substr(0, qPos);
            req.query          = req.rawQueryString;

            std::vector<std::string> pairs = split(req.rawQueryString, '&');
            for (size_t i = 0; i < pairs.size(); ++i) 
            {
                size_t eq = pairs[i].find('=');
                if (eq != std::string::npos) 
                {
                    std::string       k = pairs[i].substr(0, eq);
                    std::string       v = pairs[i].substr(eq + 1);
                    req.queryStrings[k] = v;
                }
            }
        } 
        else 
        {
            req.path = req.rawPath;
        }
    }

    while (std::getline(stream, line) && line != "\r") 
    {
        size_t pos = line.find(':');
        if (pos != std::string::npos) 
        {
            std::string     key   = line.substr(0, pos);
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
    while (std::getline(stream, line)) 
    {
        body += line + "\n";
    }
    if (!body.empty()) 
    {
        req.body = body;
    }

    size_t          lastSlash = req.path.find_last_of('/');
    if (lastSlash != std::string::npos && lastSlash + 1 < req.path.length()) 
    {
        req.filename = req.path.substr(lastSlash + 1);
    } 
    else 
    {
        req.filename = "index.html"; 
    }
    req.complete = true;

    return 0;
}


void HttpRequest::printHttpRequest(const HttpRequest& req) 
{
    std::cout << GREEN << "===== HttpRequest Detail =====\n" << RESET;
    std::cout << GREEN << "request:" << RESET << "\n"       << req.request         << "\n";
    std::cout << "\n";

    std::cout << GREEN << std::left << std::setw(22) << "method:" << RESET         << req.method         << "\n";
    std::cout << "\n";

    std::cout << GREEN << std::left << std::setw(22) << "rawPath:" << RESET         << req.rawPath        << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "path:" << RESET            << req.path           << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "filename:" << RESET        << req.filename       << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "rawQueryString:" << RESET  << req.rawQueryString << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "query:" << RESET           << req.query          << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "queryStrings:" << RESET << "\n";
    for (std::map<std::string, std::string>::const_iterator it = req.queryStrings.begin(); 
        it != req.queryStrings.end(); 
        ++it) 
    {
        std::cout << "  - " << it->first << " = " << it->second << "\n";
    }
    std::cout << "\n";


    std::cout << GREEN << std::left << std::setw(22) << "version:" << RESET         << req.version        << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "afterVersion:" << RESET    << req.afterVersion   << "\n";
    std::cout << "\n";


    std::cout << GREEN << std::left << std::setw(22) << "headers:" << RESET << "\n";
    for (std::map<std::string, std::string>::const_iterator it = req.headers.begin(); 
        it != req.headers.end(); 
        ++it) 
    {
        std::cout << "  - " << it->first << ": " << it->second << "\n";
    }
    std::cout << GREEN << std::left << std::setw(22) << "contentLength:" << RESET   << req.contentLength  << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "contentType:" << RESET     << req.contenttype    << "\n";
    std::cout << GREEN << std::left << std::setw(22) << "cookie:" << RESET          << req.cookie         << "\n";
    std::cout << "\n";


    std::cout << GREEN << std::left << std::setw(22) << "rawBody size:" << RESET << req.rawBody.size() << " bytes\n";
    std::cout << GREEN << std::left << std::setw(22) << "body:" << RESET << "\n" << req.body << "\n";


    std::cout << GREEN << std::left << std::setw(22) << "complete:" << RESET        << (req.complete ? "true" : "false") << "\n";
    std::cout << GREEN << "===============================\n" << RESET;


    std::cout << GREEN << std::left << std::setw(18) << "rawBody (hex):" << RESET;
    for (size_t i = 0; i < req.rawBody.size(); ++i) 
    {
        if (i % 16 == 0) std::cout << "\n  ";
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)req.rawBody[i] << " ";
    }
    std::cout << std::dec << "\n";
    std::cout << GREEN << "===============================\n" << RESET;


    
    std::cout << GREEN << std::left << std::setw(18) << "rawBody (hex):" << RESET   << "\n";
    for (size_t i = 0; i < req.rawBody.size(); i += 16) 
    {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
        for (size_t j = 0; j < 16 && i + j < req.rawBody.size(); ++j) 
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)req.rawBody[i + j] << " ";
        }
        std::cout << " | ";
        for (size_t j = 0; j < 16 && i + j < req.rawBody.size(); ++j) 
        {
            char c = req.rawBody[i + j];
            std::cout << (std::isprint(c) ? c : '.');
        }
        std::cout << "\n";
    }

    std::cout << std::dec; // reset base
    std::cout << GREEN << "===============================\n" << RESET;
}

