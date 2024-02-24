# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 12:48:47 by hdagdagu          #+#    #+#              #
#    Updated: 2024/02/24 10:31:30 by rrhnizar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3
NAME = webserv
OBJDIR = .objFiles

SRC_DIR = src
INCLUDE_DIR = include
CONFIG_INCLUDE_DIR = $(SRC_DIR)/config/include
SERVER_INCLUDE_DIR = $(SRC_DIR)/server/include
CGI_INCLUDE_DIR = $(SRC_DIR)/CGI/include
REQUEST_RESPONSE = $(SRC_DIR)/Request_Response/include

SRCS = main.cpp \
	$(SRC_DIR)/config/src/CommonDirectives.cpp \
	$(SRC_DIR)/config/src/CustomException.cpp \
	$(SRC_DIR)/config/src/Location.cpp \
	$(SRC_DIR)/config/src/Parser.cpp \
	$(SRC_DIR)/config/src/StringExtensions.cpp \
	$(SRC_DIR)/config/src/Server.cpp \
	$(SRC_DIR)/server/src/Wb_Server.cpp \
	$(SRC_DIR)/server/src/SendTracker.cpp \
	$(SRC_DIR)/server/src/request.cpp \
	$(SRC_DIR)/CGI/src/CGI.cpp \
	$(SRC_DIR)/Request_Response/src/Request.cpp \
	$(SRC_DIR)/Request_Response/src/Response.cpp \
	$(SRC_DIR)/Request_Response/src/Main_Response.cpp \
	$(SRC_DIR)/Request_Response/src/Response_Utils.cpp \
	$(SRC_DIR)/Request_Response/src/AutoIndex.cpp \
	$(SRC_DIR)/Request_Response/src/Client_Errors.cpp 

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = -I$(INCLUDE_DIR) -I$(CONFIG_INCLUDE_DIR) -I$(SERVER_INCLUDE_DIR) -I$(CGI_INCLUDE_DIR) -I$(REQUEST_RESPONSE)

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


$(NAME): $(OBJS)
	@echo "$(RED)$(NAME)$(YELLOW) Executable is ready$(NC)"
	@$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	@echo "$(GREEN)Compiling $(notdir $<)$(NC) >> $(GREEN)$(notdir $@)$(NC)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning up object files$(NC)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Removing executable $(NAME)$(NC)"
	@rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

watch:
	@npx nodemon -x 'reset && make run' -e cpp,hpp



.PHONY: all clean fclean re ascii_art
