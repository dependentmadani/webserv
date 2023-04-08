# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/19 15:27:10 by mbadaoui          #+#    #+#              #
#    Updated: 2023/04/08 14:27:09 by sriyani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = webserv

CPP = c++

CPPFLAGS =  -g3 -fsanitize=address -std=c++98 -Wall -Werror -Wextra

SRC = main.cpp\
		parse/parsing.cpp\
		# parse/request_parsing.cpp

HEADERS = parse/parsing.hpp
			

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