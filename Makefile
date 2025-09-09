COLOR_RESET	=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
CYAN		= 	\033[1;36m


CC			=	c++
# CFLAGS		=	-Wall -Werror -Wextra 
CFLAGS		=	-Wall -Werror -Wextra -std=c++98 -pedantic

#########################################################
NAME		=	Server

HEADER		=	./include/HttpRequest.hpp \
				./include/RouteConfig.hpp \
				./include/ServerConfig.hpp \
				./include/Connection.hpp \
				./include/ConnectionManager.hpp\
				./include/Server.hpp\
				./include/RequestException.hpp \
				./include/HttpResponse.hpp


SRC_FILES	=	./src/main.cpp \
				./src/Run_server/HttpRequest.cpp \
				./src/Set_server/RouteConfig.cpp \
				./src/Set_server/ServerConfig.cpp \
				./src/Set_server/Server.cpp \
				\
				./src/Run_server/ConnectionManager.cpp \
				./src/Run_server/Connection.cpp \
				./src/Run_server/HttpResponse.cpp



FOLDER_1	=	Response
FOLDER_2	=	Run_server
FOLDER_3	=	Set_server
FOLDER_4	=	

PORT 		= 	9081
PORT2 		= 	8888
PORT3 		= 	7777

HOST 		= 	localhost
CONFIG		=	config/default.conf

#########################################################
OBJ_DIR		=	obj
OBJ_FILES   = $(notdir $(SRC_FILES:.cpp=.o))         
OBJ_TARGETS = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))  

all: $(NAME)  

$(NAME): $(OBJ_TARGETS) $(HEADER) Makefile
	$(CC) $(CFLAGS) $(OBJ_TARGETS) -o $@
	@echo "$(YELLOW)✅ make all finish$(COLOR_RESET)"


$(OBJ_DIR)/%.o: %.cpp $(HEADER) Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# สำหรับ FOLDER SRC
$(OBJ_DIR)/%.o: src/%.cpp $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# สำหรับ FOLDER_1/
$(OBJ_DIR)/%.o: src/$(FOLDER_1)/%.cpp $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# สำหรับ FOLDER_2/
$(OBJ_DIR)/%.o: src/$(FOLDER_2)/%.cpp $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# สำหรับ FOLDER_3/
$(OBJ_DIR)/%.o: src/$(FOLDER_3)/%.cpp $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# สำหรับ FOLDER_4/
$(OBJ_DIR)/%.o: src/$(FOLDER_4)/%.cpp $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -rf $(OBJ_DIR);
	@echo "$(CYAN)✅ make clean finish$(COLOR_RESET)"

fclean: clean
	rm -f $(NAME);
	@echo "$(CYAN)✅ make fclean finish$(COLOR_RESET)"	

re: fclean all
	@echo "$(GREEN)✅ make re finish$(COLOR_RESET)"		

.PHONY: all clean fclean re




#########################################################

t: all clean
	@echo "$(GREEN)✅ run program$(COLOR_RESET)"
	./Server $(CONFIG)


# ✅ Success expected (2xx)
# Target	❓ Test อะไร	✅ 								ควรได้อะไร
# t51		POST ไปยัง Python CGI script (/session/)		200 OK พร้อมเนื้อหา HTML จาก sessionTest.py
# t52		GET PHP CGI (/delete/delete.php)				200 OK และ response จาก delete.php
# t53		GET / → โหลด index.html							200 OK และ HTML หน้าแรก
# t54		Autoindex ที่ /uploads/							200 OK พร้อม HTML รายการไฟล์ใน dir
# t55		POST HTML content ไปที่ /uploads				201 Created พร้อมข้อความ “Upload OK”
# t56		POST binary ไฟล์ .ico ไปยัง /uploads			201 Created (แสดงว่าไฟล์ binary ถูกอัปโหลด)
# t57		GET /uploads/s2.txt ที่มีอยู่จริง					200 OK และเนื้อหาใน s2.txt
# t58		DELETE /uploads/s2.txt ที่มีอยู่					204 No Content (สำเร็จ)
# t59		GET CGI Python /cgi-bin/processPlayer.py		200 OK และผลลัพธ์จาก CGI
# t60		POST ไปยัง Python CGI (/cgi-bin/processPlayer.py)	200 OK และ CGI ตอบกลับด้วยผลลัพธ์จากฟอร์ม

# 🔁 Redirect expected (3xx)
# Target	❓ Test อะไร	✅ 									ควรได้อะไร
# t61		GET /redirect → redirect ไป /					302 Found พร้อม header Location: /
# t62		GET /session → redirect ไป /session/			302 Found พร้อม header Location: /session/

# 🔴 Error expected (4xx)
# Target	❓ Test อะไร	✅ 									ควรได้อะไร
# t63		GET /../etc/passwd → path traversal				400 Bad Request (ถูกบล็อก) หรือ 403
# t64		DELETE /uploads/notfound.txt ที่ไม่มีอยู่				404 Not Found
# t65		GET ไฟล์ /notfound.txt ที่ไม่มีอยู่						404 Not Found และโชว์ custom error page (errors/404.html)



t51:
	curl -v -X POST "http://$(HOST):$(PORT)/session/" \
		-H "Content-Type: application/x-www-form-urlencoded" \
		--data "username=kit"

t52:
	curl -v -X GET "http://$(HOST):$(PORT)/delete/delete.php"

t53:
	curl -v -X GET "http://$(HOST):$(PORT)/"

t54:
	curl -v -X GET "http://$(HOST):$(PORT)/uploads/"

t55:
	curl -v -X POST "http://$(HOST):$(PORT)/uploads" \
		-H "Content-Type: text/html" \
		--data "<h1>Hello World</h1>"

t56:
	curl -v -X POST "http://$(HOST):$(PORT)/uploads" \
		--data-binary @wwwroot/www1/favicon.ico

t57: 
	curl -v -X GET "http://$(HOST):$(PORT)/uploads/s2.txt"
#t57  1aaa  3aaa


t58:
	curl -v -X DELETE "http://$(HOST):$(PORT)/uploads/s2.txt"

t59:
	curl -v -X GET "http://$(HOST):$(PORT)/cgi-bin/processPlayer.py"

t60:
	curl -v -X POST "http://$(HOST):$(PORT)/cgi-bin/processPlayer.py" \
		-H "Content-Type: application/x-www-form-urlencoded" \
		--data "name=kit&score=42"




t61:
	curl -v -X GET "http://$(HOST):$(PORT)/redirect"

t62:
	curl -v -X GET "http://$(HOST):$(PORT)/session"





t63:
	curl -v -X GET "http://$(HOST):$(PORT)/../etc/passwd"

t64:
	curl -v -X DELETE "http://$(HOST):$(PORT)/uploads/notfound.txt"

t65:
	curl -v -X GET "http://$(HOST):$(PORT)/notfound.txt"

















#target: send GET request พร้อม query + header
t01:
	curl -v -X GET "http://$(HOST):$(PORT)/search?q=webserv&lang=en" \
		-H "User-Agent: curl/7.68.0" \
		-H "Accept: */*" \
		-H "Cookie: PHPSESSID=xyz789"

#GET ธรรมดา
t02:
	curl http://$(HOST):$(PORT)/

# GET พร้อม Query String
t03:
	curl "http://$(HOST):$(PORT)/search?q=webserv&lang=en"

# GET พร้อม Header (เช่น Cookie, User-Agent)
t04:
	curl -H "User-Agent: MyBrowser/1.0" \
	     -H "Cookie: PHPSESSID=xyz789" \
	     http://$(HOST):$(PORT)/profileobj

# POST แบบ form-urlencoded
t05:
	curl -X POST \
	     -H "Content-Type: application/x-www-form-urlencoded" \
	     -d "username=admin&password=1234" \
	     http://$(HOST):$(PORT)/login

# POST แบบ JSON
t06:
	curl -X POST \
	     -H "Content-Type: application/json" \
	     -d '{"username": "kit", "email": "kit@webserv.com"}' \
	     http://$(HOST):$(PORT)/api/use

#POST พร้อมหลาย header
t07:
	curl -X POST \
	     -H "Content-Type: application/json" \
	     -H "Authorization: Bearer mytoken123" \
	     -d '{"action": "test"}' \
	     http://$(HOST):$(PORT)/api/test

# DELETE request
t08:
	curl -X DELETE http://$(HOST):$(PORT)/files/test.txt

# PUT (อัปโหลดไฟล์หรืออัปเดตข้อมูล)
t09:
	curl -X PUT \
	     -H "Content-Type: text/plain" \
	     --data-binary @file.txt \
	     http://$(HOST):$(PORT)/uploads
#	     http://$(HOST):$(PORT)/upload/file.txt	

# Upload แบบ multipart/form-data
t10:
	curl -X POST \
	     -F "file=@image.jpg" \
	     http://$(HOST):$(PORT)/upload

# ดู Header Response เท่านั้น (ไม่สน body)
t11:
	curl -I http://$(HOST):$(PORT)/

# Verbose (ดูทั้ง request/response แบบ raw)
t12:
	curl -v http://$(HOST):$(PORT)/

# ใช้ curl เพื่อทดสอบ 404 หรือ 403
t13:
	curl http://$(HOST):$(PORT)/secret

# ใช้ curl เพื่อทดสอบ 404 หรือ 403
t14:
	curl http://$(HOST):$(PORT)/this-does-not-exist

# ใช้ curl เพื่อทดสอบ 404 หรือ 403 ..chat ใส่ 2 บรรทัด??
t15:
	curl http://$(HOST):$(PORT)/secret
	curl http://$(HOST):$(PORT)/this-does-not-exist

# t16 <======
# ใช้ curl ส่ง HTTP request ครบ headers/body/query    ==>full_request
t16:
	curl -v -X POST "http://$(HOST):$(PORT)/search?q=webserv&lang=en" \
		-H "Host: $(HOST):$(PORT)" \
		-H "User-Agent: MakefileTest/1.0" \
		-H "Accept: */*" \
		-H "Cookie: PHPSESSID=xyz789; theme=dark" \
		-H "Content-Type: application/json" \
		-d '{"keyword": "webserv", "category": "network"}'

# เพิ่ม target สำหรับดูเฉพาะ header response		==>only_headers:
t17:
	curl -I http://$(HOST):$(PORT)/

# ทดสอบ 404		==>notfound:
t18:
	curl -v http://$(HOST):$(PORT)/doesnotexist

# ทดสอบ Forbidden	==>forbidden:
t19:
	curl -v http://$(HOST):$(PORT)/secret

# แถม log ตอน test  ชื่อไฟล์ ==> curl_response.txt
t20:
	curl -v http://$(HOST):$(PORT)/ > curl_response.txt 2>&1


t21:
	curl -v http://$(HOST):$(PORT)/redirect

# แนะนำ test case เพิ่มเติม
# URL			คาดหวังผล
# /redirect	→ 301 + Location: /
# /moved		→ 302 + Location: /new
# /wrong		→ 404
# /			→ 200 (OK + HTML page)

t22:
	bash test_get.sh


t23:
	curl -v -X GET "http://$(HOST):$(PORT)/search?q=webserv&lang=en" \
		-H "User-Agent: curl/7.68.0" \
		-H "Accept: */*" \
		-H "Cookie: PHPSESSID=xyz789"


# --- Basic GET ---
t24:
	curl -v -X GET "http://$(HOST):$(PORT)/index.html"

# --- POST Form ---
t25:
	curl -v -X POST "http://$(HOST):$(PORT)/login" \
		-H "Content-Type: application/x-www-form-urlencoded" \
		--data "username=kit&password=1234"

# --- POST Upload ---
t26:
	curl -v -X POST "http://$(HOST):$(PORT)/upload" \
		-F "file=@test_data/hello.txt"

# --- PUT Request ---
t27:
	curl -v -X PUT "http://$(HOST):$(PORT)/data/user.json" \
		-H "Content-Type: application/json" \
		--data '{"name": "kit", "email": "kit@42.fr"}'

# --- DELETE ---
t28:
	curl -v -X DELETE "http://$(HOST):$(PORT)/uploads/delete_me.jpg"

# --- HEAD ---
t29:
	curl -v -X HEAD "http://$(HOST):$(PORT)/index.html"

# --- OPTIONS ---
t30:
	curl -v -X OPTIONS "http://$(HOST):$(PORT)/upload"

# --- Invalid Method ---
t31:
	@echo -e "GETTT / HTTP/1.1\r\nHost: $(HOST):$(PORT)\r\n\r\n" | nc $(HOST) $(PORT)

# --- Not Found ---
t32:
	curl -v -X GET "http://$(HOST):$(PORT)/no_such_file.html"

# --- Redirect ---
t33:
	curl -v -X GET "http://$(HOST):$(PORT)/old-path" -L

# --- CGI Example ---
t34:
	curl -v -X GET "http://$(HOST):$(PORT)/script.php"

# --- Too Large ---
t35:
	@perl -e 'print "A"x10000000' > bigfile.txt
	curl -v -X POST "http://$(HOST):$(PORT)/upload" \
		-H "Content-Type: application/octet-stream" \
		--data-binary "@bigfile.txt"

# --- Chunked ---
t36:
	@echo -e "POST /upload HTTP/1.1\r\nHost: $(HOST):$(PORT)\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n" | nc $(HOST) $(PORT)




# กรณีที่เราจะทดสอบ
# ✅ Path ผิดปกติ (เช่น ..) → 400 Bad Request
# ✅ Redirect → 3xx
# ✅ Method ไม่อนุญาต → 405
# ✅ POST ไม่มี Content-Length → 411
# ✅ POST ใหญ่เกิน → 413
# ✅ DELETE ไฟล์มีจริง → 204
# ✅ DELETE ไฟล์ไม่มีอยู่ → 403
# ✅ Upload OK → 201
# ✅ CGI → รันสำเร็จ
# ✅ Autoindex เปิด → แสดงลิสต์
# ✅ Static file → 200 OK

# 🔧 หมายเหตุ
# upload, cgi-bin/hello.py, dir_with_autoindex/, 
# และ static/index.html ต้องมีอยู่ในเซิร์ฟเวอร์จริง
# อย่าลืม chmod +x ให้ CGI script ด้วย
# แนะนำให้คุณใช้ curl -L เพื่อทดสอบ redirect ด้วย
# หากใช้ epoll, โปรดตรวจสอบว่า server พร้อมรับหลายคำขอ (non-blocking I/O)

# www/
# ├── static/
# │   └── index.html               ← ใช้ใน t47 และ redirect
# ├── uploads/
# │   └── testfile.txt            ← ใช้ใน t42
# ├── auto/
# │   ├── file1.txt               ← ใช้ใน t46 (autoindex)
# │   └── file2.txt
# ├── cgi-bin/
# │   └── hello.py                ← ใช้ใน t45

t37:
	curl -v -X GET "http://$(HOST):$(PORT)/../etc/passwd"

t38:
	curl -v -L -X GET "http://$(HOST):$(PORT)/redirect" 

t39:
	curl -v -X PUT "http://$(HOST):$(PORT)/index.html"

t40:
	curl -v -X POST "http://$(HOST):$(PORT)/upload"

t41:
	curl -v -X POST "http://$(HOST):$(PORT)/upload" \
		-H "Content-Length: 9999999999"

t42:
	touch testfile.txt
	curl -v -X DELETE "http://$(HOST):$(PORT)/uploads/testfile.txt"

t43:
	curl -v -X DELETE "http://$(HOST):$(PORT)/uploads/non_exist.txt"

t44:
	curl -v -X POST "http://$(HOST):$(PORT)/upload" \
		-H "Content-Length: 20" \
		-d "Hello from Webserv!"

t45:
	curl -v -X GET "http://$(HOST):$(PORT)/cgi-bin/hello.py"

t46:
	curl -v -X GET "http://$(HOST):$(PORT)/dir_with_autoindex/"

t47:
	curl -v -X GET "http://$(HOST):$(PORT)/static/index.html"

t48:
	curl -v -X GET "http://$(HOST):$(PORT)/not_found.txt"






















# Default target
# all: t01 t02 t03 t04 t05 t06 t07

# ---------- Tests ----------

# t01: ส่ง GET ปกติ
y01:
	curl -v \
	     http://$(HOST):$(PORT)/

# t02: ส่ง POST แต่ไม่ส่ง Content-Length
y02:
	curl -v -X POST \
	     -H "Content-Type: text/plain" \
	     --data-binary "no content length" \
	     http://$(HOST):$(PORT)/upload

# t03: ส่ง Content-Length ไม่ตรงกับ body
y03: #seg fault
	curl -v -X POST \
	     -H "Content-Type: text/plain" \
	     -H "Content-Length: 100" \
	     --data-binary "short body" \
	     http://$(HOST):$(PORT)/upload

# t04: ส่ง Method ที่ server ไม่รองรับ
y04:
	curl -v -X PATCH \
	     -H "Content-Type: text/plain" \
	     --data "patching..." \
	     http://$(HOST):$(PORT)/

# t05: path แปลก ๆ หรือผิด format
y05:
	curl -v -X GET \
	     http://$(HOST):$(PORT)//bad//path???

# t06: Header format ผิด (ใช้ -H แบบไม่มี ':')
y06:
	curl -v -X GET \
	     -H "BrokenHeader" \
	     http://$(HOST):$(PORT)/

# t07: JSON request พร้อม Bearer Token
y07:
	curl -v -X POST \
	     -H "Content-Type: application/json" \
	     -H "Authorization: Bearer mytoken123" \
	     -d '{"action": "test"}' \
	     http://$(HOST):$(PORT)/api/test

# t08: Body ใหญ่มากเกิน client_max_body_size
y08: #seg fault
	dd if=/dev/zero bs=1k count=200 > /tmp/bigbody.txt
	curl -v -X POST \
	     -H "Content-Type: application/octet-stream" \
	     --data-binary @/tmp/bigbody.txt \
	     http://$(HOST):$(PORT)/upload
	rm -f /tmp/bigbody.txt



y09:
	curl -v -X WRONGMETHOD \
	     -H "Content-Type: application/json" \
	     -H "Authorization: Bearer mytoken123" \
	     -d '{"action": "test"}' \
	     http://$(HOST):$(PORT)/
