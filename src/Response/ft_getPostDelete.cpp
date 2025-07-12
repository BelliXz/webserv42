

// #include "HttpRequest.hpp"
#include "../../include/All.hpp"


void ft_sendResponse(int client_fd, 
                    int status_code, 
                    const std::string& status_text,
                    const std::string& body, 
                    const std::string& content_type)
{

    if (status_code == 200)
    {
        // ✅ ถ้า OK (status 200) → ตอบ HTML กลับ
        std::ostringstream html;
        // html << "<!DOCTYPE html>\n"
        //      << "<html><body><h1>Hello from Webserv!</h1></body></html>\n";
        html << HTML_200_TEMPLATE;
        std::string body = html.str();
        std::ostringstream res;
        res << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << body.size() << "\r\n"
            << "Connection: close\r\n"
            << "\r\n"
            << body;
        send(client_fd, res.str().c_str(), res.str().size(), 0);
    }

    std::ostringstream res;
    res << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
        << "Content-Type: " << content_type << "\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << body;

    std::string response = res.str();
    send(client_fd, response.c_str(), response.size(), 0);
}

void ft_handleGet(int client_fd, const HttpRequest& req) 
{
    std::cout<<RED;
    std::cout<<"====Handle GET====\n";
    std::cout<<RED<<RESET;

    (void)req;
    std::string html = "<html><body><h1>Hello from GET!</h1></body></html>";
    ft_sendResponse(client_fd, 200, "OK", html, "text/html");
}

void ft_handlePost(int client_fd, const HttpRequest& req) 
{
    std::string response = "<html><body><h2>POST Received:</h2><pre>" + req.body + "</pre></body></html>";
    ft_sendResponse(client_fd, 200, "OK", response, "text/html");
}

// void ft_handleDelete(int client_fd, const HttpRequest& req) 
// {
//     std::string result = "Requested to delete: " + req.path;
//     ft_sendResponse(client_fd, 200, "OK", result, "text/plain");
// }

void ft_handleDelete(int client_fd, const HttpRequest& req) 
{
    // สร้าง path ที่จะลบ (อิงว่า root directory = "./www/html/")
    std::string root = "www/html";
    std::string filepath = root + req.path;

    std::ostringstream body;
    int status_code;
    std::string status_text;

    // พยายามลบไฟล์
    if (std::remove(filepath.c_str()) == 0) 
    {
        // ✅ ลบสำเร็จ
        status_code = 200;
        status_text = "OK";
        body << "✅ Deleted: " << req.path << "\n";
    } 
    else 
    {
        // ❌ ลบไม่สำเร็จ (ไฟล์ไม่เจอหรือไม่มีสิทธิ์)
        status_code = 404;
        status_text = "Not Found";
        body << "❌ Failed to delete: " << req.path << "\n";
    }

    // ส่ง response กลับ
    std::ostringstream res;
    res << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
        << "Content-Type: text/plain\r\n"
        << "Content-Length: " << body.str().size() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << body.str();

    send(client_fd, res.str().c_str(), res.str().size(), 0);
}
