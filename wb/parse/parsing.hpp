/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:23 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/08 15:36:33 by sriyani          ###   ########.fr       */
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
    std::string root_location;
    std::string url_location;
    std::vector<std::string> location;
    std::vector<std::string> methods;
    std::vector<std::string> index;
    std::vector<std::string> cgi_pass;
    std::string return_url;
    int num_return;
    bool auto_index;
    int location_flag;
    
} location;

typedef struct s_server
{
    private:
    public:
    int num_location;
    int ind_port;
    unsigned long max_client;
    std::vector<std::string> server;
    std::string server_name;
    std::string host;
    std::vector<std::string>  error_page;
    std::vector<int>  error_num;
    std::vector<int>  lent_server;
    location **loc;
    int server_flag;
} t_server;


typedef struct s_parsing
{
    private:
    // std::vector<std::string> server_name;
    public:
    int num_serv;
    int pars_flag;
    int count_flag;
    std::vector<std::string> vec;
    t_server **serv;
    void    copy_file(s_parsing *pars, std::string ptr);
    void    check_key(s_parsing *pars);
    void check_server(s_parsing *pars, size_t len);
    void check_listen(t_server *pars, std::string str);
    void check_server_name(t_server *serv, std::string str);
    void check_error_pages(t_server *serv, std::string str);
    void check_max_client(t_server *serv, std::string str);
    void check_location(location *loc);
    std::string trim(const std::string& str);
    
} parsing;
bool whitespace(unsigned char c);
bool    isWhitespace(unsigned char c);

int ft_len(std::string s, char c);
// char	*ft_substr(char *s, unsigned int start, size_t len);
// int	ft_len(char  *s, char c);
// char	**ft_split(char  *s, char c);

#endif