# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/17 17:06:06 by kchaouki          #+#    #+#              #
#    Updated: 2024/01/30 12:16:36 by rrhnizar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS = main.cpp \
	   ConfigParsing2/StringExtensions.cpp \
	   ConfigParsing2/CustomException.cpp \
	   ConfigParsing2/CommonDirectives.cpp \
	   ConfigParsing2/Location.cpp \
	   ConfigParsing2/Server.cpp \
	   ConfigParsing2/Parser.cpp \
	   Request_Response/TmpServer.cpp \
	   Request_Response/Request.cpp \
	   Request_Response/Response.cpp

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