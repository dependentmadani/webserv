/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/30 17:31:52 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parsing.hpp"
#include <string.h>



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

void parsing::check_listen(t_server *pars, std::string str)
{
    std::string ptr;
    // std::string s = "scott>=tiger>=mushroom";
    std::string delimiter = " ";
    
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        std::cout <<"++++++++|"<<token <<"+++++++"<< std::endl;
    }
    // std::cout <<str << std::endl;
     
    // std::cout <<"|||||" <<tokens[0] << "|||||||"<< std::endl;
    
    
}
void parsing::check_server(s_parsing *pars, int len)
{
    for (int i = 0; i < len ; i++)
    {
       for (int j = 0; j < pars->serv[i]->server.size(); j++)
       {
        size_t found;
        found = pars->serv[i]->server[j].find("listen");
        if (found != std::string::npos)
            {check_listen(pars->serv[i], pars->serv[i]->server[j]);break ;}
            
        pars->serv[i]->server[j].erase(std::remove_if(pars->serv[i]->server[j].begin(),pars->serv[i]->server[j].end(), whitespace), pars->serv[i]->server[j].end());
        if (!strncmp(pars->serv[i]->server[j].c_str(), "}", 1))
            break ;
        // else
        //     std::cout <<" ERRRRRROR "<<std::endl;
        // std::cout<< j << " ************ |"<<pars->serv[i]->server[j]<<"|  ************"<<std::endl;
        // if (pars->serv[i]->server[j])
       }
    }
    
    // check_server_name(pars);
    // check_error_pages(pars);
    // check_client_max_size(pars);
}

void  parsing::check_key(s_parsing *pars)
{
    int k = 0;
    
    for(int i=0;i < pars->vec.size() ;i++)
    {
        pars->vec[i].erase(std::remove_if(pars->vec[i].begin(), pars->vec[i].end(), whitespace), pars->vec[i].end());
        if (!strncmp(pars->vec[i].c_str(), "server", 6) || pars->vec[i] == "\0")
        {
            // std::cout<< i << " ************ |"<< pars->vec[i]<<"|  ************"<<std::endl;
            if(!strncmp(pars->vec[i].c_str(), "server", 6))
            {
                pars->serv[k] = new t_server();
                for(int j = i+1; j < pars->vec.size() ;j++)
                {
                    if (!strncmp(pars->vec[j].c_str(),"}",1))
                    {
                        pars->serv[k]->server.push_back(pars->vec[j]);
                        break ;
                    }
                    else
                        pars->serv[k]->server.push_back(pars->vec[j]);
                }
                k++;
            }
        }
        // else
        //     std::cout<< i << " Error "<<std::endl;
    }
   check_server(pars, k);
    
}

std::vector<std::string>  parsing::get_vec()
{
    return this->vec;
}