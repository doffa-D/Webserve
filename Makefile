C++ = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp\
	Server.cpp

OBJ = $(SRC:.cpp=.o)

NAME = webserv

all : $(NAME)

$(NAME) : $(OBJ)
	@$(C++) $(FLAGS) $(OBJ) $(OPTS) -o $(NAME)

%.o : %.cpp
	@$(C++) $(FLAGS) -c $< -o $@

clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re:fclean $(NAME)