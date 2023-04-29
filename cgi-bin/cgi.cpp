/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/29 16:19:04 by sriyani          ###   ########.fr       */
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
    _pwd = str;
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
   _cgi_script = const_cast<char*> (serv->loc[0]->cgi_pass[1].c_str());
    
}
void CGI::handle_cgi_request(Request& req)
{
    char **ptr =  new char *[2];
    std::string tmp = _pwd;
    tmp +="/";
    tmp += _cgi_script+2;
    char *pp = const_cast<char*> (tmp.c_str());
    ptr[0] = pp;
    ptr[1] = NULL;
    
    std::stringstream ss;
    std::string str = ss.str();
    std::ofstream file("file.txt");
    // file << req.getBody(); 
    (void) req;
    file.close();
    int fd = open("file.txt", O_RDONLY, 644);
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    // std::cerr << "Error forking process" << std::endl;
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