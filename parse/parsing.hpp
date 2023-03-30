/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:23 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/30 12:31:59 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef PARSING_HPP
# define PARSING_HPP
typedef struct s_server
{
    std::vector<std::string> server;
    int ind_port;
    int ind_host;
    int error_pages;
    int limit_body;
    std::string server_name;
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
    void check_server(s_parsing *pars);
    std::vector<std::string>  get_vec();
    
} parsing;
bool whitespace(unsigned char c);
bool    isWhitespace(unsigned char c);



#endif