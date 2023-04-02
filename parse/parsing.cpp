/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/02 17:34:25 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parsing.hpp"
#include <string.h>
#include <sstream>  


int ft_len(std::string s, char c)
{
    int len;
    int i =0;

    len = 0;
    while (s[i])
    {
        if ((s[i] != c && len == 0) || (s[i] != c && (s[i-1] ) == c))
            len++;
        i++;
    }
    return (len);
}

std::string parsing::trim(const std::string& str)
{
    std::size_t first = str.find_first_not_of(" \t\n\r\f\v");
    std::size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
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
    char *ss;
    std::string s2;
    str = trim(str);
    if (!strncmp(str.c_str(), "listen",6))
    {

            str = str.c_str()+6;
            str = trim(str);
            ss = const_cast<char*>(str.c_str());
            ptr = strtok(ss, ":");
            while ( ptr != NULL)
            {
                if (isNumber(ptr))
                    serv->ind_port = atoi(ptr);
                else
                    serv->host = ptr;
                ptr = strtok(NULL,  " ");
            }
    }
}

void parsing::check_server_name(t_server *serv, std::string str)
{
    char  *ptr;
    char *ss;
    std::string s2;
    str = trim(str);
    if (!strncmp(str.c_str(), "server_name",11))
    {
        str = str.c_str()+11;
        str = trim(str);
        serv->server_name = str;
    }
}


void parsing::check_error_pages(t_server *serv, std::string str)
{
    char  **ptr;
    char *ss;
    char * s2;
    str = trim(str);
    int lent =  0;
    int j = 10;
    int k; 
    if (!strncmp(str.c_str(), "error_page",10))
    {
        lent = ft_len(str,' ');
        lent = ft_len(str, 9)+lent;
        // std::cout<<"-------|"<< lent<<"|>>>>>>>>>>>>"<<std::endl;
        ss = const_cast<char*>(str.c_str());
        for (int i = 0; i < lent -1; i++)
        {
            for ( ; ss[j] == ' ' || ss[j]  == 9 ; j++)
            {
            }
            k = 0;
            for (; ss[j] ; j++)
            {
                if (ss[j] == ' ')
                {
                    break;
                }
                
                s2[k] = ss[j];
                std::cout<<"-------|"<< s2[k] <<"|>>>>>>>>>>>>"<<std::endl;
                    k++;
            }
            s2[k] = '\0';
            std::string po(s2);
            std::cout<<po<<"|*******************|"<< s2<<std::endl;
            serv->error.push_back(po);
            
        }
         for (int i =0; i < serv->error.size(); i++)
        {
           std::cout<<">>>>>>>>>>>>"<<serv->error[i]<<std::endl;
        }
        
    }

}

void parsing::check_max_client(t_server *serv, std::string str)
{
    char *ss;
    str = trim(str);
    if (!strncmp(str.c_str(), "max_client_body_size",20))
    {
            str = str.c_str()+21;
            str = trim(str);
            ss = const_cast<char*>(str.c_str());
            if (isNumber(ss))
                serv-> max_client = atoi(ss);
             else
                std::cout<<"Error max_client_body_size"<<std::endl;
                
    }
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

        }
        found = pars->serv[i]->server[j].find("server_name");
        if (found != std::string::npos)
        {
            check_server_name(pars->serv[i], pars->serv[i]->server[j]);
        }
        found = pars->serv[i]->server[j].find("max_client_body_size");
        if (found != std::string::npos)
        {
            check_max_client(pars->serv[i], pars->serv[i]->server[j]);
        }
        found = pars->serv[i]->server[j].find("error_page");
        if (found != std::string::npos)
        {
            check_error_pages(pars->serv[i], pars->serv[i]->server[j]);
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
        if (!strcmp(pars->vec[i].c_str(), "server") || pars->vec[i] == "\0")
        {
            // std::cout<< i << " ************ |"<< pars->vec[i]<<"|  ************"<<std::endl;
            if(!strcmp(pars->vec[i].c_str(), "server"))
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