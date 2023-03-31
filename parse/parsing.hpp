/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:23 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/31 16:09:23 by sriyani          ###   ########.fr       */
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
    int error_pages;
    int limit_body;
    public:
    int ind_port;
    std::vector<std::string> server;
    std::string server_name;
    std::string host;
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
    void check_server_name(t_server *serv, std::string str, int pos);
    std::vector<std::string>  get_vec();
    
} parsing;
bool whitespace(unsigned char c);
bool    isWhitespace(unsigned char c);



#endif