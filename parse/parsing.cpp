/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/31 17:34:02 by sriyani          ###   ########.fr       */
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

bool isNumber(char * str)
{
    for (int i =0;str[i];i++) 
    {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
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

void parsing::check_listen(t_server *serv, std::string str)
{
    char  *ptr;
    char *ss = const_cast<char*>(str.c_str());
    char *s1;
    ptr = strtok(ss, " ");
    if (!strcmp(str.c_str(), "listen"))
    {
         while (ptr!= NULL)
        {
            if (isNumber(s1))
               serv->ind_port = atoi(s1);
            else
                serv->host = s1;
            ptr = strtok(NULL,  " ");
            s1 =  strtok(ptr,":");
        }
    }
}
void parsing::check_server_name(t_server *serv, std::string str, int pos)
{
    char  *ptr;
    int  i = 0;
    char *ss = const_cast<char*>(str.c_str());
    ptr = ss+pos;
    // std::cout <<" +++++++++" << ptr <<std::endl;
    if (!strncmp(ptr, "server_name", 11))
    {
		// i = 11;
         while (ptr[i])
        {
          	// std::cout <<" +++++++++" << ptr[i] <<std::endl;
            ptr = strtok(NULL,  " ");
			i++;
        }
    }
    // std::cout <<" +++++++++" << serv->server_name <<std::endl;
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
        {
            check_listen(pars->serv[i], pars->serv[i]->server[j]);
            // break ;
        }

        found = pars->serv[i]->server[j].find("server_name");
        if (found != std::string::npos)
        {
            check_server_name(pars->serv[i], pars->serv[i]->server[j], found);
        }
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
    pars->serv = new t_server*[pars->vec.size()];
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