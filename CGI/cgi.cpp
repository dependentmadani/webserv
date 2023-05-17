/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/16 16:20:30 by sriyani          ###   ########.fr       */
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

CGI::CGI(int loc_index, int serv_index): _location_index(loc_index), _server_index(serv_index)
{
}

CGI::~CGI()
{
}

void CGI::fill_cgi(char const *buffer, t_server *serv)
{
    std::stringstream ss(buffer);
    std::string token;

    // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<< buffer<<"|~~~~~~~~~" << std::endl;
    while (getline(ss, token, '\n'))
    {
        if (token != "\0")
        {
            hold.push_back(token);
        }
    }
    std::string s = "SCRIPT_FILENAME=";
    s +=_script_name ;
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
   check_cgi(serv->loc[_location_index]->cgi_pass);
}

void CGI::handle_cgi_request(Request& req, char const  *buffer, t_server *serv)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) 
    {
        std::cerr << "Error creating pipe" << std::endl;
        exit(1);
    }
    _script_name = req.getAvailableFilePath();
    fill_cgi(buffer, serv);
    char **ptr =  new char *[3];
    ptr[0] = const_cast<char*> (executable.c_str());
    ptr[1] = const_cast<char*>(_script_name.c_str());
    ptr[2] = NULL;
    
    std::stringstream ss;
    std::string str = ss.str();
    int fd = open("file.txt", O_CREAT| O_RDWR, 0644);
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
        exit(1);
    }
    else
    {
        close(pipe_fd[1]);
        int status;
        
        if (waitpid(pid, &status, WNOHANG) == -1)
              perror("wait() error");
    }
   
    size_t rd;
    char bufffer[2048] = " ";
    while ((rd = read(pipe_fd[0], bufffer, sizeof(buffer))) > 0)
    {
        resp_buffer += bufffer;
    }
    close(pipe_fd[0]);
    size_t found = resp_buffer.find("\r\n\r\n");
    resp_buffer = resp_buffer.substr(found+1);

    // std::cout<<"|____________|"<< resp_buffer<<"|_________|"<<std::endl;
}

std::string const& CGI::getRespBuffer() const
{
    return resp_buffer;
}
void CGI::check_cgi(std::vector<std::string> str)
{

   std::vector<std::string>::iterator it = str.begin();
    std::string hold = _script_name; 
    _ext = hold.erase(0, hold.find_last_of('.'));
    int  i = 0;
    if (_ext.empty())
        return ;
    while (it != str.end())
    {
        if (*it == _ext.c_str()+1)
        {
            executable = str[i+1];
            break ;
        }
        ++it;
        ++i;
    }
    
}

