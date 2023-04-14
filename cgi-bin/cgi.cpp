/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/14 17:30:48 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "cgi.hpp"
#include <stdlib.h>
#include <sstream>

CGI::CGI(/* args */)
{
}

CGI::~CGI()
{
}
void CGI::fill_cgi(char *buffer, t_server *serv)
{
    int len =0;
    (void)serv;
    for(int i = 0;buffer[i];i++)
    {
        if (buffer[i]=='\n')
            len++;
    }
    
    std::cout<<"||||||||||||||||||||"<<len<<std::endl;
    
}
void CGI::handle_cgi_request(int fd)
{
    char **ptr =  new char *;
    ptr[0] = const_cast<char*> (_cgi_name.c_str());
    ptr[1] = NULL;
    
    std::stringstream ss;
    ss << _http_host;
    std::string str = ss.str();

    
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        (void) fd;
        setenv("REQUEST_METHOD", _methode.c_str() , 1);
        setenv("SCRIPT_FILENAME", _cgi_name.c_str(), 1);
        setenv("QUERY_STRING", "", 1);
        
        setenv("HTTP_HOST", _http_host.c_str(), 1);
        setenv("SERVER_NAME", _server_name.c_str() , 1);
        setenv("SERVER_PORT", str.c_str() , 1);
        char*  x = getenv("SERVER_PORT"); 
        std::cout<<"|           |"<< _cgi_name <<"|      |"<< x <<std::endl;
        execve(ptr[0], ptr, NULL);
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