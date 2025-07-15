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
NAME		=	webserv

HEADER		= 	./include/All.hpp \
				./include/Core.hpp \
				./include/HttpRequest.hpp \
				./include/ServerConfig.hpp 


SRC_FILES	=	./src/main.cpp \
				./src/ft_sleep.cpp \
				./src/Response/ft_checkErrorHttpRequest.cpp \
				./src/Response/ft_getPostDelete.cpp \
				./src/Response/ft_redirect.cpp \
				./src/Run_server/ft_handleClient.cpp \
				./src/Run_server/ft_parseHttpRequest.cpp \
				./src/Run_server/ft_serverLoop.cpp \
				./src/Set_server/ft_parseConfig.cpp \
				./src/Set_server/ft_setEpoll.cpp


FOLDER_1	=	Response
FOLDER_2	=	Run_server
FOLDER_3	=	Set_server
FOLDER_4	=	

PORT 		= 	9091
HOST 		= 	localhost
CONFIG		=	config/kit.conf

#########################################################
OBJ_DIR		=	obj
OBJ_FILES   = $(notdir $(SRC_FILES:.cpp=.o))         
OBJ_TARGETS = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))  

all: $(NAME)  

$(NAME): $(OBJ_TARGETS) $(HEADER) Makefile
	$(CC) $(CFLAGS) $(OBJ_TARGETS) -o $@
	@echo "$(YELLOW)✅ make all finish$(COLOR_RESET)"
	@echo "$(YELLOW)run =======> ./$(NAME) <xxx.conf>$(COLOR_RESET)"
	@echo "$(YELLOW)or  =======> make t (and other terminal make t16)$(COLOR_RESET)"


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

t: all
	./webserv $(CONFIG)


fl: all
	valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv config/kit.conf
#	valgrind --track-fds=yes --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell


#html
h200:
	xdg-open ./html/200.html 
h404:
	xdg-open ./html/404.html 
htest:
	xdg-open ./html/test.html 


#curl เป็นเครื่องมือ command-line ที่ใช้ส่ง HTTP request ไปยัง server และดู response ได้ 
#		เช่น GET, POST, PUT, DELETE ฯลฯ

#-v หรือ --verbose
#		เปิดโหมดแสดงรายละเอียด ของ request และ response

# -X หรือ --request
# 		ใช้ระบุ method ที่จะใช้กับ request (เช่น GET, POST, DELETE, PUT)
# 		ปกติ curl จะเลือก method ตามที่คุณใช้งาน เช่น:
# 		- ถ้าคุณใส่ --data → ใช้ POST อัตโนมัติ
# 		- ถ้าไม่ใส่อะไร → ใช้ GET

# -H หรือ --header
# 		เพิ่ม custom HTTP header
# 		สามารถใช้หลายอัน:
# 		มีประโยชน์มากในการทดสอบ API เช่น:
#		- เพิ่ม Content-Type, Authorization, Cookie
#		- จำลอง request แบบ browser


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
	     http://$(HOST):$(PORT)/upload/file.txt	

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




###############################################################

FOLDER  = กระดาษทด/
PROGRAM = a.out

FT_TEST = ft_vector_assign.cpp

c:
	c++ กระดาษทด/$(FT_TEST) -o $(FOLDER)$(PROGRAM)  &&  ./$(FOLDER)$(PROGRAM) 


