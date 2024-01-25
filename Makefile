# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/17 17:06:06 by kchaouki          #+#    #+#              #
#    Updated: 2024/01/25 17:24:02 by rrhnizar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS = main.cpp \
	   ConfigParsing/StringExtensions.cpp \
	   ConfigParsing/CustomException.cpp \
	   ConfigParsing/CommonDirectives.cpp \
	   ConfigParsing/Location.cpp \
	   ConfigParsing/Server.cpp \
	   ConfigParsing/Parser.cpp \
	   Request_Response/TmpServer.cpp \
	   Request_Response/Request.cpp \
	   Request_Response/Response.cpp

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

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