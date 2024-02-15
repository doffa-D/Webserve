# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 12:48:47 by hdagdagu          #+#    #+#              #
#    Updated: 2024/02/15 12:36:06 by hdagdagu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3
NAME = webserv
OBJDIR = .objFiles

SRC_DIR = src

INCLUDE_DIR = include
CONFIG_INCLUDE_DIR = $(SRC_DIR)/config/include
EXCEPTION_INCLUDE_DIR = $(SRC_DIR)/exception/include
REQUEST_INCLUDE_DIR = $(SRC_DIR)/request/include
SERVER_INCLUDE_DIR = $(SRC_DIR)/server/include

SRCS = main.cpp \
	$(SRC_DIR)/config/src/CommonDirectives.cpp \
	$(SRC_DIR)/config/src/CustomException.cpp \
	$(SRC_DIR)/config/src/Location.cpp \
	$(SRC_DIR)/config/src/Parser.cpp \
	$(SRC_DIR)/config/src/StringExtensions.cpp \
	$(SRC_DIR)/config/src/Server.cpp \
	$(SRC_DIR)/server/src/Wb_Server.cpp \
	$(SRC_DIR)/server/src/Upload.cpp \
	$(SRC_DIR)/server/src/SendTracker.cpp \
	$(SRC_DIR)/server/src/request.cpp \
	$(SRC_DIR)/CGI/src/CGI.cpp \




OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCLUDES = -I$(INCLUDE_DIR) -I$(CONFIG_INCLUDE_DIR) -I$(EXCEPTION_INCLUDE_DIR) -I$(REQUEST_INCLUDE_DIR) -I$(SERVER_INCLUDE_DIR) -I./src1/Includes

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
	@echo "$(YELLOW)Linking object files to create executable $(NAME)$(NC)"
	@$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	@echo "$(GREEN)Compiling $< and generating object file $@$(NC)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning up object files$(NC)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Removing executable $(NAME)$(NC)"
	@rm -f $(NAME)

re: fclean all

run: all
	@echo "$(YELLOW)Running $(NAME)$(NC)"
	@./$(NAME) config_file/default.conf


# watch:
#     @echo "$(YELLOW)Watching for changes...$(NC)"
#     @while true; do \
#         inotifywait -r -e modify -e create -e delete $(SRC_DIR) && \
#         make --no-print-directory run; \
#     done

	
.PHONY: all clean fclean re ascii_art
