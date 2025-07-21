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

HEADER		=	./include/ConfigParser.hpp \
				./include/HttpRequest.hpp \
				./include/RouteConfig.hpp \
				./include/ServerConfig.hpp \
				./include/Webserv.hpp 


SRC_FILES	=	./src/main.cpp \
				./src/Run_server/HttpRequest.cpp \
				./src/Run_server/ConfigParser.cpp \
				./src/Set_server/RouteConfig.cpp \
				./src/Set_server/ServerConfig.cpp \
				./src/Set_server/Webserv.cpp 




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
	./webserv $(CONFIG)