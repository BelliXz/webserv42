

// #include "HttpRequest.hpp"
#include "../../include/All.hpp"

// void ft_handleClient(int client_fd, char* buffer) 
void ft_handleClient(int client_fd, HttpRequest req) 
{
    // HttpRequest req;
    // ft_parseHttpRequest(buffer, req);  // แยกข้อมูลออกจาก raw HTTP request     <=== paser

    // std::cout << GREEN;
    // ft_printHttpRequest(req);
    // std::cout << GREEN << RESET;
    

    //////////////////////
	if (req.path == "/redirect") 
    {
		ft_redirect(client_fd, "/", 301); // → stop here  fix 301 ??????
		return;
	}
    //////////////////////


    int status = ft_checkErrorHttpRequest(req, client_fd);            // <=== check error
    if (status != 200)
        return;

        
    // <=== check CGI ?????


        
    // === เลือกตาม method ========================
    if (req.method == "GET")
        ft_handleGet(client_fd, req);
    else if (req.method == "POST")
        ft_handlePost(client_fd, req);
    else if (req.method == "DELETE")
        ft_handleDelete(client_fd, req);


    // สรุป: POST / GET / DELETE ใช้ "ตอนไหน"
    // Method	    เกิดขึ้นเมื่อ	                             คุณควรทำอะไรใน server
    // GET	        Client ขอข้อมูล เช่นไฟล์, HTML	           โหลดไฟล์ → สร้าง response body แล้วส่งกลับ
    // POST	        Client ส่งข้อมูล เช่น form หรือ upload	    อ่าน req.body → save หรือประมวลผล
    // DELETE	    Client ขอให้ลบ resource	                ลบไฟล์หรือข้อมูลตาม req.path
    
    // 🔧 ตัวอย่างจาก curl:
    // curl -X POST -d "name=chatgpt&score=99" http://localhost:8080/submit
    // curl -X GET  http://localhost:8080/
    // curl -X DELETE http://localhost:8080/file.txt




    // // ✅ ถ้า OK (status 200) → ตอบ HTML กลับ
    // std::ostringstream html;
    // // html << "<!DOCTYPE html>\n"
    // //      << "<html><body><h1>Hello from Webserv!</h1></body></html>\n";
    // html << HTML_200_TEMPLATE;
    // std::string body = html.str();
    // std::ostringstream res;
    // res << "HTTP/1.1 200 OK\r\n"
    //     << "Content-Type: text/html\r\n"
    //     << "Content-Length: " << body.size() << "\r\n"
    //     << "Connection: close\r\n"
    //     << "\r\n"
    //     << body;
    // send(client_fd, res.str().c_str(), res.str().size(), 0);//===========================
}
