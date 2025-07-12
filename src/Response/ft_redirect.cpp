#include "../../include/All.hpp"

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

