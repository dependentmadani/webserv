/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 15:26:22 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/26 15:25:25 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef PARSING_HPP
# define PARSING_HPP
class parsing
{
    private:
    std::vector<std::string> vec;
    int ind_port;
    int ind_host;
    int error_pages;
    int limit_body;
    std::string server_name;
    public:
        parsing();
        ~parsing();
    void    copy_file(std::string ptr);
    void    check_key();
    std::vector<std::string>  get_vec();
};


#endif