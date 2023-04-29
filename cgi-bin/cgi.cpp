/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/29 10:05:21 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "cgi.hpp"
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>

CGI::CGI(/* args */)
{
}

CGI::~CGI()
{
}
void CGI::fill_cgi(char *buffer, t_server *serv)
{
    std::stringstream ss(buffer);
    std::string token;

    while (getline(ss, token, '\n'))
    {
        if (token != "\0")
        {
            hold.push_back(token);
        }
    }
    char *str = getcwd(NULL, 0);
    std::string s = "SCRIPT_FILENAME=";
    s += str;
    s += "/cgi-bin";
    _envcgi.push_back(s);
    _envcgi.push_back("REDIRECT_STATUS=200");
    for(size_t j = 0;j < hold.size();j++)
    {
        
        size_t found = hold[j].find("HTTP/");
        if (found != std::string::npos)
        {
            std::string str;
            found = hold[j].find("?");
            if (found != std::string::npos)
            {
                str = "QUERY_STRING=";
                str += hold[j].substr(found + 1);
                char *s = strtok(const_cast<char*>(str.c_str()), " ");
                _envcgi.push_back(s);
            }
            found = hold[j].find("GET");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char*>(hold[j].c_str()), " ");
                str+=s;
                _envcgi.push_back(str);
            }
            found = hold[j].find("POST");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char*>(hold[j].c_str()), " ");
                str+=s;
                _envcgi.push_back(str);
            }
            found = hold[j].find("DELETE");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char*>(hold[j].c_str()), " ");
                str+=s;
                _envcgi.push_back(str);
            }
            
            found = hold[j].find("DELETE");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char*>(hold[j].c_str()), " ");
                str+=s;
                _envcgi.push_back(str);
            }
        }
        found = hold[j].find("Content-Length:");
        if (found != std::string::npos)
        {
            std::string str = "CONTENT_LENGTH=";
            str += hold[j].c_str()+16;
            _envcgi.push_back(str);
        }
        found = hold[j].find("Content-Type:");
        if (found != std::string::npos)
        {
            std::string str = "CONTENT_Type=";
            str += hold[j].c_str()+14;
            _envcgi.push_back(str);
        }
    }
    _env = new char* [_envcgi.size()];
    
    for (size_t i = 0; i < _envcgi.size(); i++)
    {
        _env[i] = new char [_envcgi[i].size()];
        _env[i]  = const_cast<char*> (_envcgi[i].c_str());
    }
    // for (size_t i = 0; i < _envcgi.size(); i++)
    // {
    //     std::cout<< "|******|"<<_env[i]<<std::endl;
    // }
   _cgi_script = const_cast<char*> (serv->loc[0]->cgi_pass[1].c_str());
    
}
void CGI::handle_cgi_request(Request& req)
{
    char **ptr =  new char *;
    std::string hh = "script.sh";
    char *pp = const_cast<char*> (hh.c_str());
    ptr[0] = pp;
    ptr[1] = NULL;
    
    std::stringstream ss;
    (void)req;
    std::string str = ss.str();
    std::ofstream file("file.txt");
    file << req.getBody(); 
    file.close();
    int fd = open("file.txt", O_RDONLY, 644);
    std::cout<<ptr[0]<<"|---------|" <<"|********|"<<req.getBody()<<std::endl;
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        dup2(fd, STDIN_FILENO);
        execve(ptr[0], ptr, _env);
        std::cerr << "Error executing CGI script" << std::endl;
        exit(1);
    }
    else 
        waitpid(pid, NULL, 0);
}
// int main(int ac , char **av, char **env)
// {
    
//     std::string str = "script.pl";
//     handle_cgi_request(str, env, 4, "POST");
// }
