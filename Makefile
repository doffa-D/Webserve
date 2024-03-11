# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 12:48:47 by hdagdagu          #+#    #+#              #
#    Updated: 2024/03/11 17:23:30 by hdagdagu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98
NAME = webserv
OBJDIR = .objFiles

UPLOAD_DIR := upload
SESSION_DIR := session
LOGS_DIR := logs

SRC_DIR = src

INCLUDE_DIR = include/header.hpp \
			$(SRC_DIR)/CGI/include/CGI.hpp\
			$(SRC_DIR)/server/include/Upload.hpp\
			$(SRC_DIR)/server/include/Upload.hpp\
			$(SRC_DIR)/server/include/Wb_Server.hpp\
			$(SRC_DIR)/config/include/CommonDirectives.hpp\
			$(SRC_DIR)/config/include/CustomException.hpp\
			$(SRC_DIR)/config/include/Location.hpp\
			$(SRC_DIR)/config/include/Parser.hpp\
			$(SRC_DIR)/config/include/Server.hpp\
			$(SRC_DIR)/config/include/StringExtensions.hpp\
			$(SRC_DIR)/Request_Response/include/Request.hpp\
			$(SRC_DIR)/Request_Response/include/Response.hpp


SRCS = main.cpp \
	$(SRC_DIR)/config/src/CommonDirectives.cpp \
	$(SRC_DIR)/config/src/CustomException.cpp \
	$(SRC_DIR)/config/src/Location.cpp \
	$(SRC_DIR)/config/src/Parser.cpp \
	$(SRC_DIR)/config/src/StringExtensions.cpp \
	$(SRC_DIR)/config/src/Server.cpp \
	$(SRC_DIR)/server/src/Wb_Server.cpp \
	$(SRC_DIR)/server/src/upload.cpp \
	$(SRC_DIR)/CGI/src/CGI.cpp \
	$(SRC_DIR)/Request_Response/src/Request.cpp \
	$(SRC_DIR)/Request_Response/src/Response.cpp \
	$(SRC_DIR)/Request_Response/src/Main_Response.cpp \
	$(SRC_DIR)/Request_Response/src/AutoIndex.cpp \
	$(SRC_DIR)/Request_Response/src/Client_Errors.cpp \
	$(SRC_DIR)/Request_Response/src/Cgi_Response.cpp \
	$(SRC_DIR)/Request_Response/src/serveRequestedResource.cpp

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)


# Color codes
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
NC = \033[0m

all: ascii_art $(NAME)

ascii_art:
	@echo "\n"
	@echo "$(RED) █     █░▓█████  ▄▄▄▄     ██████ ▓█████  ██▀███   ██▒   █▓▓█████ $(NC)"
	@echo "$(RED)▓█░ █ ░█░▓█   ▀ ▓█████▄ ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ $(NC)"
	@echo "$(RED)▒█░ █ ░█ ▒███   ▒██▒ ▄██░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   $(NC)"
	@echo "$(RED)░█░ █ ░█ ▒▓█  ▄ ▒██░█▀    ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ $(NC)"
	@echo "$(RED)░░██▒██▓ ░▒████▒░▓█  ▀█▓▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒$(NC)"
	@echo "$(RED)░ ▓░▒ ▒  ░░ ▒░ ░░▒▓███▀▒▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░$(NC)"
	@echo "$(RED)  ▒ ░ ░   ░ ░  ░▒░▒   ░ ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░$(NC)"
	@echo "$(RED)  ░   ░     ░    ░    ░ ░  ░  ░     ░     ░░   ░      ░░     ░   $(NC)"
	@echo "$(RED)    ░       ░  ░ ░            ░     ░  ░   ░           ░     ░  ░$(NC)"
	@echo "$(RED)                      ░                               ░          $(NC)"
	@echo "\n"


# 

$(NAME): $(OBJS) $(INCLUDE_DIR)
	@mkdir -p $(UPLOAD_DIR) $(SESSION_DIR) $(LOGS_DIR)
	@echo "$(RED)$(NAME)$(YELLOW) Executable is ready$(NC)"
	@$(CXX) $(CFLAGS) $(OBJS) -o $(NAME)
	

$(OBJDIR)/%.o: %.cpp
	@echo "$(GREEN)Compiling $(notdir $<)$(NC) >> $(GREEN)$(notdir $@)$(NC)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning up object files$(NC)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Removing executable and directories  $(NAME) $(UPLOAD_DIR) $(SESSION_DIR) $(LOGS_DIR)$(NC)"
	@rm -rf $(UPLOAD_DIR) $(SESSION_DIR) $(LOGS_DIR) $(NAME)

re: fclean all

.PHONY: all clean fclean re ascii_art
