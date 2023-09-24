# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/19 15:27:10 by mbadaoui          #+#    #+#              #
#    Updated: 2023/05/23 14:34:03 by sriyani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = webserv

CPP = c++

CPPFLAGS = -std=c++98 -Wall -Werror -Wextra

SRC = main.cpp\
	Server/Server.cpp \
	Request-Response/Request.cpp \
	Request-Response/get_method.cpp \
	Request-Response/delete_method.cpp \
	Request-Response/post_method.cpp \
	Request-Response/Response.cpp \
	Request-Response/method_checkers.cpp \
	CGI/cgi.cpp\
	Utils/ft_split.cpp \
	Utils/utils_functions.cpp \
	Parse/parsing.cpp \

HEADERS = Server/Server.hpp \
	Request-Response/Request.hpp \
	CGI/cgi.hpp \
	Utils/utils.hpp \
	Parse/parsing.hpp

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