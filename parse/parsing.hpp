/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:23 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/30 15:43:40 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>  


#ifndef PARSING_HPP
# define PARSING_HPP
typedef struct s_server
{
    private:
    int ind_port;
    int ind_host;
    int error_pages;
    int limit_body;
    std::string server_name;
    public:
    std::vector<std::string> server;
} t_server;

typedef struct s_parsing
{
    private:
    // std::vector<std::string> server_name;
    public:
    std::vector<std::string> vec;
    t_server **serv;
    void    copy_file(s_parsing *pars, std::string ptr);
    void    check_key(s_parsing *pars);
    void check_server(s_parsing *pars, int len);
    void check_listen(t_server *pars, std::string str);
    std::vector<std::string>  get_vec();
    
} parsing;
bool whitespace(unsigned char c);
bool    isWhitespace(unsigned char c);



#endif