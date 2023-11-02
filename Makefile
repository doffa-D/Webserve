# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 12:48:47 by hdagdagu          #+#    #+#              #
#    Updated: 2023/11/02 13:02:34 by hdagdagu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CFLAGS = -Wall -Werror -Wextra #-fsanitize=address -g 
INCLUDES = include/Server.hpp
NAME = Webserv
OBJDIR = .objFiles

SRC_DIR = src
SRCS = main.cpp Server.cpp
OBJ = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

# DEBUG
ifeq ($(DEBUG), 1)
   OPTS = -fsanitize=address -g
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(OPTS) -o $(NAME)

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	mkdir -p $(OBJDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

run: all
	./$(NAME)

re: fclean all

