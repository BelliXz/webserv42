

// #include "HttpRequest.hpp"
#include "../../include/All.hpp"


void ft_handleClient(int client_fd, char* buffer) 
{
    HttpRequest req;
    ft_parseHttpRequest(buffer, req);  // แยกข้อมูลออกจาก raw HTTP request     <=== paser

    std::cout << GREEN;
    ft_printHttpRequest(req);
    std::cout << GREEN << RESET;
    
    //////////////////////
	if (req.path == "/redirect") 
    {
		ft_redirect(client_fd, "/", 301); // → stop here  fix 301 ??
		return;
	}
    //////////////////////


    int status = ft_checkErrorHttpRequest(req, client_fd);            // <=== check error
    if (status != 200)
        return;


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



void ft_parseHttpRequest(char* buffer, HttpRequest& req) 
{
    std::string         request(buffer);
    req.rawBody.assign(buffer, buffer + strlen(buffer)); // เก็บ raw body
            // คือการ คัดลอกข้อมูลดิบ (raw data) ทั้งหมดจาก buffer 
            // ซึ่งเป็น HTTP request ที่รับเข้ามา (char*) 
            // แล้วเก็บไว้ใน std::vector<char> rawBody;
            // ซึ่งเป็นสมาชิกของ struct หรือ class HttpRequest

            ////////////////////////////////////////////////////////////// 
            // อธิบายทีละส่วน:
            // 👉 buffer:
            //     เป็น pointer (char*) ไปยังข้อความ raw ที่ได้จาก client เช่น:
            //         GET /hello HTTP/1.1\r\n
            //         Host: localhost:8080\r\n
            //         User-Agent: curl/7.81.0\r\n
            //         \r\n
            // 👉 strlen(buffer):
            //     คืนความยาวของ string นี้ (ไม่รวม \0)
            //     ใช้เพื่อบอกว่าจะ copy กี่ byte
            // 👉 buffer + strlen(buffer):
            //     เป็น pointer ไปยัง ตำแหน่งสุดท้าย ของ string
            //     ดังนั้น buffer ถึง buffer + strlen(buffer) เป็นช่วงที่ครอบคลุมเนื้อหา request
            // 👉 req.rawBody.assign(begin, end):
            //     เป็นคำสั่งของ std::vector<char> ที่ใช้ copy ข้อมูลจาก pointer ช่วง begin ถึง end
            //     เทียบได้กับ:
            //         for (const char* p = buffer; p < buffer + strlen(buffer); ++p)
            //             req.rawBody.push_back(*p);

            // ✅ สรุป: ทำไมต้องเก็บ rawBody?
            // การเก็บ rawBody เป็น vector ของ char จะช่วยให้:
            //     วิเคราะห์หรือ log ข้อมูล HTTP ที่มาจาก client ได้แม่นยำแบบดิบ
            //     รองรับข้อมูลที่ไม่ใช่ text เช่น binary file (ใน POST body)
            //     ไม่ต้องแปลงซ้ำไปมาระหว่าง std::string กับ char*
            
            // 🧠 เพิ่มเติม:
            //     ถ้า request body มี binary data (เช่น upload file) 
            //     การใช้ std::string อาจทำให้ข้อมูลเสียหาย 
            //     (เพราะ std::string ทำงานกับ null-terminated string) 
            //     → ดังนั้น std::vector<char> เหมาะกว่า
            
            // 🔧 ตัวอย่าง:
            //     char buffer[] = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
            //     HttpRequest req;
            //     req.rawBody.assign(buffer, buffer + strlen(buffer));
                
            //     // req.rawBody now contains: {'G','E','T',' ','/',' ',...,'\r','\n','\r','\n'}
            ////////////////////////////////////////////////////////////// 


    std::istringstream  stream(request);
    std::string         line;
            // สร้าง stream สำหรับอ่านทีละบรรทัดจาก request

    // 1. Parse request line
    if (std::getline(stream, line)) {
        std::istringstream reqLine(line);
        reqLine >> req.method >> req.path >> req.version;
            // example
            //     GET /hello?name=kit HTTP/1.1
            //     → จะได้:
            //     req.method  = "GET";
            //     req.path    = "/hello?name=kit";
            //     req.version = "HTTP/1.1";


        // แยก query ออก (ถ้ามี ?)
        size_t qPos = req.path.find('?');
        if (qPos != std::string::npos) {
            req.query = req.path.substr(qPos + 1);
            req.path = req.path.substr(0, qPos);
        }
            // ถ้ามี ? ใน path (query string) → แยก query กับ path ออก
            // เช่น /hello?name=kit →
            //     req.path = "/hello"
            //     req.query = "name=kit"

    }

    // 2. Parse headers
    while (std::getline(stream, line) && line != "\r") {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // ft_trim spaces
            key.erase(key.find_last_not_of(" \r\n") + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" \r\n") + 1);

            req.headers[key] = value;
                    // วนอ่าน headers ทีละบรรทัดจนเจอบรรทัดว่าง (\r)
                    // แยกชื่อ header เช่น Content-Type: text/html
                    // เก็บใส่ map req.headers["Content-Type"] = "text/html"


            // ตรวจ header พิเศษ:
            // ดึง header ที่สำคัญใส่ใน field ของ HttpRequest เพิ่มเติม
            if (key == "Content-Length")
                req.contentLength = std::atoi(value.c_str());
            else if (key == "Content-Type")
                req.contenttype = value;
            else if (key == "Cookie")
                req.cookie = value;
        }
    }

    // 3. Parse body (ถ้ามี)
    // (กรณี POST หรือมีเนื้อหา)
    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }

    if (!body.empty()) {
        req.body = body;
    }
        // อ่านบรรทัดที่เหลือทั้งหมดหลัง header → รวมเป็น body
        // เก็บใน req.body



    // 4. ชื่อไฟล์ (จาก path)
    size_t lastSlash = req.path.find_last_of('/');
    if (lastSlash != std::string::npos && lastSlash + 1 < req.path.length()) {
        req.filename = req.path.substr(lastSlash + 1);
    } else {
        req.filename = "index.html"; // default
    }
        // เช่น /images/logo.png → filename = "logo.png"
        // ถ้าไม่มีชื่อไฟล์ → ใช้ "index.html" เป็นค่าเริ่มต้น

 
    // ✅ ตั้งค่า complete บอกว่า request นี้ parse เสร็จเรียบร้อย
    req.complete = true;
}
    // ตัวอย่างผลลัพธ์ที่ได้จากการ parse
    // จาก input:
/*
POST /upload/photo.jpg?token=abc123 HTTP/1.1
Host: localhost:8080
Content-Type: image/jpeg
Content-Length: 1024
Cookie: session=xyz

<binary body here>
*/

    // จะได้ใน req:
    //     req.method        = "POST"
    //     req.path          = "/upload/photo.jpg"
    //     req.query         = "token=abc123"
    //     req.version       = "HTTP/1.1"
    //     req.headers[...]  = { ... }
    //     req.contenttype   = "image/jpeg"
    //     req.contentLength = 1024
    //     req.cookie        = "session=xyz"
    //     req.filename      = "photo.jpg"
    //     req.body          = "<binary body>"
    //     req.complete      = true



void ft_printHttpRequest(const HttpRequest& req) {
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
        std::cout << "  - " << it->first << ": " << it->second << "\n";
    }

    std::cout << std::left << std::setw(18) << "body:" << "\n" << req.body << "\n";

    std::cout << std::left << std::setw(18) << "rawBody size:"   << req.rawBody.size() << " bytes\n";
    std::cout << "===============================\n";
}




void ft_sendResponse(int client_fd, int status_code, const std::string& status_text,
                      const std::string& body, const std::string& content_type)
{
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
