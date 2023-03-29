/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/29 17:29:50 by sriyani          ###   ########.fr       */
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

void parsing::copy_file(parsing *pars, std::string ptr)
{
    std::string line;
    std::ifstream file(ptr);
    
    int i = 0;
    while (getline(file, line))
        pars->vec.push_back(line);
     
}
void  parsing::check_key(s_parsing *pars)
{
    int j =0;
    // pars->serv = new t_server;
    for(int i=0;i < pars->vec.size() ;i++)
    {
        if (!strncmp(pars->vec[i].c_str(), "server", 6) || pars->vec[i] == "\0" || isWhitespace(pars->vec[i]))
        {
            if(!strncmp(pars->vec[i].c_str(), "server", 6) && !strncmp(pars->vec[i+1].c_str(), "{", 1))
            {
                for(int j = i+2; j < pars->vec.size() ;j++)
                {
                    if (!strncmp(pars->vec[j].c_str(),"}",1))
                        break ;
                    else
                        pars->serv[j].server.push_back(pars->vec[j]);
                }
                j++;
                // break;
            }
        }
    }
    std::cout<<"-------"<<pars->serv[0].server.size()<<"---------"<<std::endl;
    for (size_t i = 0; i < pars->serv[0].server.size() ; i++)
    {
        std::cout<<"-------"<<pars->serv[0].server[i]<<"---------"<<std::endl;
    }
    
}

std::vector<std::string>  parsing::get_vec()
{
    return this->vec;
}