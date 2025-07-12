
// #include "HttpRequest.hpp"
#include "../../include/All.hpp"


// คำอธิบายสั้นสำหรับสถานะหลัก:
// Code	หมายถึง
// 200	OK (สำเร็จ)
// 201	Created (POST แล้วมีผลลัพธ์)
// 204	No Content
// 301	Moved Permanently
// 302	Found (redirect ชั่วคราว)
// 400	Bad Request
// 401	Unauthorized (ต้อง login/cookie)
// 403	Forbidden
// 404	Not Found
// 405	Method Not Allowed
// 411	Length Required (ไม่มี Content-Length)
// 413	Payload Too Large
// 500	Internal Server Error
// 504	Gateway Timeout (timeout)



int ft_checkErrorHttpRequest(const HttpRequest& req, int client_fd) 
{
    // กลุ่ม 4xx: Client Error
    // Code	    ความหมาย
    // 400	    Bad Request
    // 401	    Unauthorized
    // 403	    Forbidden
    // 404	    Not Found
    // 405	    Method Not Allowed
    // 411	    Length Required
    // 413	    Payload Too Large

    // ✅ 1. ตรวจ 400 Syntax Error → missing method/path/version
    if (req.method.empty() || req.path.empty() || req.version.empty()) {
        std::string res =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Bad Request";
        send(client_fd, res.c_str(), res.size(), 0);
        return 400;
    }

    // ✅ 2. ตรวจ 405 Method Not Allowed
    if (req.method != "GET" && req.method != "POST" && req.method != "DELETE") {
        std::string res =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Allow: GET, POST, DELETE\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 18\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Method Not Allowed";
        send(client_fd, res.c_str(), res.size(), 0);
        return 405;
    }

    // ✅ 3. ตรวจ 411 Length Required → POST ต้องมี Content-Length
    if (req.method == "POST" && req.contentLength == 0) {
        std::string res =
            "HTTP/1.1 411 Length Required\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 16\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Length Required";
        send(client_fd, res.c_str(), res.size(), 0);
        return 411;
    }

    // ✅ 4. ตรวจ 413 Payload Too Large
    if (req.contentLength > 1000000) {  // เช่น 1MB limit
        std::string res =
            "HTTP/1.1 413 Payload Too Large\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 19\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Payload Too Large";
        send(client_fd, res.c_str(), res.size(), 0);
        return 413;
    }

    // ✅ 5. ตรวจ 403 Forbidden
    if (req.path == "/secret" || req.path.find("/admin") == 0) {
        std::string res =
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 9\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Forbidden";
        send(client_fd, res.c_str(), res.size(), 0);
        return 403;
    }

    // ✅ 6. ตรวจ 404 Not Found (จำกัดเฉพาะ path ที่อนุญาต)
    if (req.path != "/" && req.path != "/hello" && req.path != "/search") {
        std::ostringstream html;
        html << "<html><body><h1>404 Not Found</h1></body></html>";
        std::string body = html.str();
        std::ostringstream res;
        res << "HTTP/1.1 404 Not Found\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << body.size() << "\r\n"
            << "\r\n"
            << body;
        send(client_fd, res.str().c_str(), res.str().size(), 0);
        return 404;
    }

    // ✅ 7. ตรวจ 401 Unauthorized (ถ้าไม่มี Cookie)
    if (req.path == "/cookie" && req.cookie.empty()) {
        std::string res =
            "HTTP/1.1 401 Unauthorized\r\n"
            "WWW-Authenticate: Cookie realm=\"webserv\"\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 12\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Unauthorized";
        send(client_fd, res.c_str(), res.size(), 0);
        return 401;
    }


    // กลุ่ม 5xx: Server Error
    // Code	ความหมาย
    // 500	    Internal Server Error
    // 504	    Gateway Timeout

    // ✅ 8. ตัวอย่าง 504 Gateway Timeout (mock)
    if (req.path == "/timeout") {
        std::string res =
            "HTTP/1.1 504 Gateway Timeout\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 16\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Gateway Timeout";
        send(client_fd, res.c_str(), res.size(), 0);
        return 504;
    }

    // ✅ 9. ตัวอย่าง 500 Internal Server Error (mock path)
    if (req.path == "/crash") {
        std::string res =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 21\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Internal Server Error";
        send(client_fd, res.c_str(), res.size(), 0);
        return 500;
    }


    // กลุ่ม 3xx: Redirection
    // Code     ความหมาย
    // 301	    Moved Permanently
    // 302	    Found (Temporary Redirect)
    

    if (req.path == "/redirect") {
        std::string res =
            "HTTP/1.1 301 Moved Permanently\r\n"
            "Location: /\r\n\r\n";
        send(client_fd, res.c_str(), res.size(), 0);
        return 301;
    }

    

    // กลุ่ม 2xx: Success
    // Code	ความหมาย	                    เงื่อนไขการใช้
    // 200	OK	                            ใช้เมื่อทุกอย่างเรียบร้อย
    // 201	Created	                        ใช้กับ POST ที่สร้าง resource ใหม่ เช่น /upload
    // 202	Accepted	                    รับไว้แล้ว แต่ยังไม่ได้ประมวลผล เช่น async task
    // 203	Non-Authoritative Information	เช่น proxy ตอบกลับแทนต้นทาง
    // 204	No Content	                    ไม่มี body กลับมา เช่น DELETE สำเร็จ

    if (req.path == "/upload" && req.method == "POST") {
        std::string res =
            "HTTP/1.1 201 Created\r\n"
            "Content-Length: 0\r\n\r\n";
        send(client_fd, res.c_str(), res.size(), 0);
        return 201;
    }
    
    if (req.path == "/accepted") {
        std::string res =
            "HTTP/1.1 202 Accepted\r\n"
            "Content-Length: 0\r\n\r\n";
        send(client_fd, res.c_str(), res.size(), 0);
        return 202;
    }


    // ✅ ตอบปกติ 200 OK
    return 200;
}






// int ft_checkErrorHttpRequest______old(const HttpRequest& req, int client_fd) {
//     // ✅ 1. ตรวจ 400 Syntax (ไม่มี method/path/version)
//     if (req.method.empty() || req.path.empty() || req.version.empty()) {
//         std::string res =
//             "HTTP/1.1 400 Bad Request\r\n"
//             "Content-Type: text/plain\r\n"
//             "Content-Length: 11\r\n"
//             "Connection: close\r\n"
//             "\r\n"
//             "Bad Request";
//         send(client_fd, res.c_str(), res.size(), 0);
//         return 400;
//     }

//     // ✅ 2. ตรวจ 405 Method ที่ไม่รองรับ
//     if (req.method != "GET" && req.method != "POST" && req.method != "DELETE") {
//         std::string res =
//             "HTTP/1.1 405 Method Not Allowed\r\n"
//             "Allow: GET, POST, DELETE\r\n"
//             "Content-Type: text/plain\r\n"
//             "Content-Length: 18\r\n"
//             "Connection: close\r\n"
//             "\r\n"
//             "Method Not Allowed";
//         send(client_fd, res.c_str(), res.size(), 0);
//         return 405;
//     }

//     // ✅ 3. ตรวจ 403 path ที่ต้องห้าม  ตรวจ path ที่ห้ามเข้าถึง 
//     if (req.path == "/secret") {
//         std::string res =
//             "HTTP/1.1 403 Forbidden\r\n"
//             "Content-Type: text/plain\r\n"
//             "Content-Length: 9\r\n"
//             "Connection: close\r\n"
//             "\r\n"
//             "Forbidden";
//         send(client_fd, res.c_str(), res.size(), 0);
//         return 403;
//     }

//     // ✅ 4. ตรวจ 404 path ไม่พบ ตรวจว่า path มีหรือไม่มี
//     if (req.path != "/" && req.path != "/hello") {
//         std::ostringstream html;
//         html << HTML_404_TEMPLATE;
//         std::string body = html.str();
//         std::ostringstream res;
//         res << "HTTP/1.1 404 Not Found\r\n"
//             << "Content-Type: text/html\r\n"
//             << "Content-Length: " << body.size() << "\r\n"
//             << "\r\n"
//             << body;
//         send(client_fd, res.str().c_str(), res.str().size(), 0);
//         return 404;
//     }

//     // ✅ OK → ไม่มี error
//     return 200;
// }


void ft_redirect(int client_fd, const std::string& location, int status_code) {
	std::ostringstream res;
	std::string status_text;

	switch (status_code) 
    {
		case 301: status_text = "Moved Permanently"; break;
		case 302: status_text = "Found"; break;
		case 303: status_text = "See Other"; break;
		case 307: status_text = "Temporary Redirect"; break;
		case 308: status_text = "Permanent Redirect"; break;
		default:
			status_code = 302;
			status_text = "Found";
			break;
	}

	res << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
	    << "Location: " << location << "\r\n"
	    << "Content-Length: 0\r\n"
	    << "Connection: close\r\n"
	    << "\r\n";

	std::string response = res.str();
	send(client_fd, response.c_str(), response.size(), 0);

	std::cout << "[Redirect] " << status_code << " → " << location << "\n";
}

// เคสการใช้งานจริง
// URL เดิม	              ต้อง redirect ไป	 เหตุผล
// /old-home	             /new-home	      เปลี่ยนชื่อ path
// /redirect	             /	              ใช้ทดสอบ
// /index.html	             /	              ทำให้ URL สั้นลง
// /login (หลัง POST สำเร็จ)	/dashboard	     ส่ง user ไปหน้าถัดไป

