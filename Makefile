# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/19 15:27:10 by mbadaoui          #+#    #+#              #
#    Updated: 2023/04/28 15:05:55 by sriyani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = webserv

CPP = c++

CPPFLAGS = -fsanitize=address -std=c++98 -Wall -Werror -Wextra

SRC = main.cpp\
	Server/Server.cpp \
	Request/Request.cpp \
	utils/ft_split.cpp \
	utils/utils_functions.cpp \
	parse/parsing.cpp \
	cgi-bin/cgi.cpp\
	

HEADERS = Server/Server.hpp \
	Request/Request.hpp \
	utils/utils.hpp \
	parse/parsing.hpp \
	cgi-bin/cgi.hpp\

OBJ = $(SRC:.cpp=.o)

all : $(NAME)
	

$(NAME): $(OBJ) $(HEADERS)
		$(CPP) $(CPPFLAGS) $(SRC)  -o $(NAME)

clean: 
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re