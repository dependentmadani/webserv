/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/11 10:54:46 by sriyani          ###   ########.fr       */
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
    if (str)
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
    _env = new char* [_envcgi.size()+1];
    for (size_t i = 0; i < _envcgi.size(); i++)
        _env[i]  = const_cast<char*> (strdup(_envcgi[i].c_str()));
    _env[_envcgi.size()] = NULL;
   _cgi_script =  serv->loc[0]->cgi_pass[1].c_str();

    
}
void CGI::handle_cgi_request(Request& req)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) 
    {
        std::cerr << "Error creating pipe" << std::endl;
        exit(1);
    }
    char **ptr =  new char *[2];
    std::string tmp = _pwd;
    tmp += "/";
    tmp += _cgi_script.c_str() + 2;
    ptr[0] = const_cast<char*> (tmp.c_str());

    ptr[1] = NULL;
    
    std::stringstream ss;
    std::string str = ss.str();

    int fd = open("file.txt", O_CREAT| O_RDWR, 644);
    write(fd, req.getBody().c_str(),req.getBody().length());

    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execve(ptr[0], ptr, _env);
        std::cerr << "Error executing CGI script" << std::endl;
        // std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
        exit(1);
    }
    else
    {
        close(pipe_fd[1]);
        int status;
        waitpid(pid, &status, 0);
    }
   
    size_t rd;
    char buffer[2048] = " ";
    // fd = open("file.txt", O_RDONLY, 644);
    while ((rd = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
    {
        resp_buffer += buffer;
    }
    // close(fd);
    close(pipe_fd[0]);
}

