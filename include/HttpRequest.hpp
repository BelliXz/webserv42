
#include "Core.hpp"

class HttpRequest 
{
    public:

        std::string             filename;                   // ✅ ชื่อไฟล์ที่ถูกเรียก เช่น "/images/logo.png" → "logo.png"
        std::string             method;                     // ✅ HTTP method เช่น "GET", "POST", "DELETE"
        std::string             path;                       // ✅ URL path เช่น "/", "/login", "/images/logo.png"
        std::string             query;                      // ✅ ส่วน query string หลัง "?" เช่น "?id=5&name=foo"     //&
        std::string             version;                    // ✅ HTTP version เช่น "HTTP/1.1"
        std::string             body;                       // ✅ เนื้อหาของ request (ใช้กับ POST/PUT)
        std::vector<char>       rawBody;                    // ✅ สำเนา raw body (ใช้เก็บ binary หรือ upload ได้)
        std::string             contenttype;                // ✅ ค่า Content-Type จาก header เช่น "text/html", "application/json"
        std::string             cookie;                     // ✅ ค่า Cookie จาก header เช่น "PHPSESSID=abc123"
        std::map<std::string, std::string> headers;         // ✅ เก็บ header ทุกบรรทัดเป็น key-value เช่น {"Host": "localhost"}
        bool                    complete;       // = false  // ✅ ใช้ระบุว่า parsing สมบูรณ์ไหม
        size_t                  contentLength;  // = 0      // ✅ ค่าจาก header "Content-Length" (จำนวน byte ใน body)


        HttpRequest() 
            : complete(false), contentLength(0) 
        {}
};



// อธิบายส่วนประกอบของ HTTP Request ที่เกี่ยวข้อง:
// ✅ 1. Request Line
//     GET /hello?id=5 HTTP/1.1

// method  → "GET"
// path    → "/hello"
// query   → "id=5"
// version → "HTTP/1.1"


// ✅ 2. Headers
//     Host: localhost:8080
//     Content-Type: text/html
//     Content-Length: 45
//     Cookie: session=abc123

// เก็บใน:
// headers["Host"] = "localhost:8080"
// contenttype = "text/html"
// cookie = "session=abc123"
// contentLength = 45


// ✅ 3. Body (เช่น POST/PUT)
//     name=kit&email=kit@example.com

// เก็บใน:
// body = "name=kit&email=kit@example.com"
// rawBody เก็บในรูป binary ด้วย (เช่น upload ไฟล์)

// ✅ 4. Extra: filename
//     หาก path คือ /images/logo.png → filename = "logo.png"
//     ถ้า path = / → filename = "index.html" (default fallback)


// 💡 การใช้งานจริง:
// คุณสามารถใช้โครงสร้างนี้เพื่อ:

// ใช้ทำอะไร	          ตัวแปรที่เกี่ยว
// Routing	            method + path
// Auth/Cookie	        headers + cookie
// โหลด static file	    filename
// ตรวจ body upload	    contentLength + contenttype + rawBody
// log request	        headers + method + path
// ตรวจ error	        ใช้ method/path/version/check headers

// 📌 สรุป
// Field	                ประโยชน์
// method, path, version	ใช้ routing และตรวจ format
// headers	                เก็บข้อมูลสำคัญ เช่น Content-Type, Cookie, Content-Length
// body	                    ใช้ใน POST, PUT
// rawBody	                รองรับ binary เช่น upload ไฟล์
// contenttype	            ใช้บอกชนิดข้อมูล เช่น application/json
// cookie	                ใช้สำหรับ session หรือ auth
// filename	                ใช้โหลด static file
// complete	                เช็กว่าพร้อมใช้งานหรือยัง
// query	                ใช้แยกพารามิเตอร์ใน URL



// ตัวอย่าง HTTP Request หลายกรณี ทั้ง GET, POST, DELETE 
// พร้อมคำอธิบาย และ mapping กับ field ใน HttpRequest class ของคุณ

// 🟢 1. GET Request (ทั่วไป)
//     GET /home HTTP/1.1
//     Host: localhost:8080
//     User-Agent: curl/7.68.0
//     Accept: */*

// Field	                ค่า
// method	                "GET"
// path	                "/home"
// version	                "HTTP/1.1"
// headers["Host"]	        "localhost:8080"
// headers["User-Agent"]	"curl/7.68.0"
// body	                (ว่าง)
// query	                (ว่าง)
// complete	            true


// 🟡 2. GET Request with Query Parameters
//     GET /search?q=hello&page=2 HTTP/1.1
//     Host: localhost:8080

// Field	                ค่า
// path	                "/search"
// query	                "q=hello&page=2"
// filename	            "search"


// 🔵 3. POST Request (Form Data)
//     POST /login HTTP/1.1
//     Host: localhost:8080
//     Content-Type: application/x-www-form-urlencoded
//     Content-Length: 29

//     username=admin&password=1234

// Field	        ค่า
// method	        "POST"
// path	        "/login"
// contenttype	    "application/x-www-form-urlencoded"
// contentLength	29
// body	        "username=admin&password=1234"
// rawBody	        แบบ binary

// 🔴 4. POST JSON
//     POST /api/data HTTP/1.1
//     Host: localhost:8080
//     Content-Type: application/json
//     Content-Length: 45

//     {"name": "kit", "email": "kit@example.com"}

// Field	        ค่า
// contenttype	    "application/json"
// body	        JSON string

// 🟤 5. DELETE Request
//     DELETE /user/123 HTTP/1.1
//     Host: localhost:8080

// Field	        ค่า
// method	        "DELETE"
// path	        "/user/123"
// filename	    "123"


// 🟣 6. Forbidden (ใช้ path ที่ไม่อนุญาต)
//     GET /secret HTTP/1.1
//     Host: localhost:8080

// Field	        ค่า
// path	        "/secret"
// → ส่งกลับ	        403 Forbidden


// ⚫️ 7. Malformed Request (ผิด syntax)
//     GIBBERISH data here

// ผลที่ควรเป็น	    400 Bad Request
// เพราะ method, path, version แยกไม่ได้จาก request line	

// 🟠 8. Method Not Allowed
//     UT /update HTTP/1.1
//     Host: localhost:8080

// | method | "PUT" |
// | → ส่งกลับ | 405 Method Not Allowed (รองรับเฉพาะ GET, POST, DELETE) |

// 🟢 9. With Cookie
//     GET /dashboard HTTP/1.1
//     Host: localhost:8080
//     Cookie: sessionid=xyz123; theme=dark

// Field	        ค่า
// cookie	        "sessionid=xyz123; theme=dark"

// 💡 Tip: ในการทดสอบ: ใช้ curl:
//     curl -X POST -d "user=abc" http://localhost:8080/login

// หรือเขียน request มือใน telnet/nc:
//     nc localhost 8080
//     แล้วพิมพ์ตาม format



