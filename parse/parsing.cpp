/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/30 13:16:43 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parsing.hpp"
#include <string.h>

bool isWhitespace(std::string str)
{
    int len = 0;
    for (int i = 0; i < str.length(); i++)
    {
          if ((str[i] == '\t' || str[i] == '\n' || str[i] == '\r' ||  str[i] == '\f' || str[i] == '\v' || str[i] == ' '))
            len++;
    }
    if (str.length() == len)
        return  true;
    return false;
}
bool whitespace(unsigned char c) 
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
        return true;
    } else {
        return false;
    }
}
void parsing::copy_file(parsing *pars, std::string ptr)
{
    std::string line;
    std::ifstream file(ptr);
    
    int i = 0;
    while (getline(file, line))
        pars->vec.push_back(line);
     
}
// void check_server(parsing *pars)
// {
//     check_listen(pars);
//     check_server_name(pars);
//     check_error_pages(pars);
//     check_client_max_size(pars);
// }

void  parsing::check_key(s_parsing *pars)
{
    int k = 0;
    
    for(int i=0;i < pars->vec.size() ;i++)
    {
        pars->vec[i].erase(std::remove_if(pars->vec[i].begin(), pars->vec[i].end(), whitespace), pars->vec[i].end());
        if (!strncmp(pars->vec[i].c_str(), "server", 6) || pars->vec[i] == "\0")
        {
            std::cout<< i << " ************ |"<< pars->vec[i]<<"|  ************"<<std::endl;
            if(!strncmp(pars->vec[i].c_str(), "server", 6))
            {
                pars->serv[k] = new t_server();
                for(int j = i+2; j < pars->vec.size() ;j++)
                {
                    if (!strncmp(pars->vec[j].c_str(),"}",1))
                        break ;
                    else
                        pars->serv[k]->server.push_back(pars->vec[j]);
                }
                k++;
            }
        }
        // else
        //     std::cout<< i << " Error "<<std::endl;
    }
//    check_server(pars);
    
}

std::vector<std::string>  parsing::get_vec()
{
    return this->vec;
}