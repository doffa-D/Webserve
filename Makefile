# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 12:48:47 by hdagdagu          #+#    #+#              #
#    Updated: 2023/11/06 17:39:39 by hdagdagu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98
NAME = WebServer
OBJDIR = .objFiles

SRC_DIR = src
INCLUDE_DIR = include
CONFIG_INCLUDE_DIR = $(SRC_DIR)/config/include
EXCEPTION_INCLUDE_DIR = $(SRC_DIR)/exception/include
REQUEST_INCLUDE_DIR = $(SRC_DIR)/request/include
SERVER_INCLUDE_DIR = $(SRC_DIR)/server/include

SRCS = main.cpp $(SRC_DIR)/server/src/Server.cpp $(SRC_DIR)/request/src/parseHttpRequest.cpp $(SRC_DIR)/config/src/ServerConfigFile.cpp $(SRC_DIR)/exception/src/exception.cpp
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCLUDES = -I$(INCLUDE_DIR) -I$(CONFIG_INCLUDE_DIR) -I$(EXCEPTION_INCLUDE_DIR) -I$(REQUEST_INCLUDE_DIR) -I$(SERVER_INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

