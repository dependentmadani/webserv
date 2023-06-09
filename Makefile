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

CPPFLAGS = -std=c++98 -Wall -Werror -Wextra  -g #-fsanitize=address

SRC = main.cpp\
	Server/Server.cpp \
	Request/Request.cpp \
	CGI/cgi.cpp\
	utils/ft_split.cpp \
	utils/utils_functions.cpp \
	parse/parsing.cpp \

HEADERS = Server/Server.hpp \
	Request/Request.hpp \
	CGI/cgi.hpp \
	utils/utils.hpp \
	parse/parsing.hpp \

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