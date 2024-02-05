# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/17 17:06:06 by kchaouki          #+#    #+#              #
#    Updated: 2024/02/05 09:31:55 by hdagdagu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS = main.cpp \
	   StringExtensions.cpp \
	   CustomException.cpp \
	   CommonDirectives.cpp \
	   Location.cpp \
	   Server.cpp \
	   Parser.cpp

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

CCe = c++

all: $(NAME)

%.o : %.cpp
	$(CCe) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CCe) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean 
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re