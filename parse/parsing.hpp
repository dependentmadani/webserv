/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:23 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/04 13:29:24 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>  


#ifndef PARSING_HPP
# define PARSING_HPP

typedef struct s_location
{
    std::string root_locaton;
    std::vector<std::string> location;
    std::vector<std::string> methods;
    bool index;
    
} location;

typedef struct s_server
{
    private:
    public:
    int ind_port;
    unsigned long max_client;
    std::vector<std::string> server;
    std::string server_name;
    std::string host;
    std::vector<std::string>  error_page;
    std::vector<int>  error_num;
    std::vector<int>  lent_server;
    location *loc;
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
    void check_server_name(t_server *serv, std::string str);
    void check_error_pages(t_server *serv, std::string str);
    void check_max_client(t_server *serv, std::string str);
    void check_location(t_server *serv, std::string str);
    std::vector<std::string>  get_vec();
    std::string trim(const std::string& str);
    
} parsing;
bool whitespace(unsigned char c);
bool    isWhitespace(unsigned char c);

int ft_len(std::string s, char c);
// char	*ft_substr(char *s, unsigned int start, size_t len);
// int	ft_len(char  *s, char c);
// char	**ft_split(char  *s, char c);

#endif