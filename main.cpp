/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/10 18:49:05 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "Server/Server.hpp"
#include "Request/Request.hpp"
#include "parse/parsing.hpp"
#include "cgi-bin/cgi.hpp"


int main(int ac, char **av)
{
    std::ifstream file;
    Request request;
    parsing vars;
    parsing *pars = new parsing();
    CGI cgi;

    
    if (ac == 2)
    {
        file.open(av[1]);
        if (file)
        {
            vars.copy_file(pars, av[1]);
            vars.check_key(pars);
            std::cout << " File exist"<<std::endl;
        }
        else
        {
          std::cout << " File doesn't exist"<<std::endl;       
            return (1);
        }
    }
    else
    {
        std::cout<<"Error from number of arguments, make sure to have something as follows \"./webserv config_file.conf\""<<std::endl;
        exit(1);
    }
    Server  server(pars->serv[0]->ind_port);
    struct pollfd theOne;
    memset(&theOne, 0, sizeof(theOne));
    server.setParse(pars);
    if (server.initiat_server() < 0)
        return (1);
    server.accept_connections();
    theOne.fd = (server.getSocket_client())[0];
    theOne.events = POLLIN;
    server.recv_data(&theOne);
    request.ft_http_code();
    request.ft_mime_type();
    request.setParse(pars);
    request.ParseRequest(server.getBuffer());
    request.UseMethod();
    request.build_response();
    int e = send(server.getSocket_client()[0] , request.Response.c_str(), strlen(request.Response.c_str()), 0);
    (void)e;
    // std::cout << "|****************************************|" << std::endl;
    // cgi.fill_cgi(server.getBuffer(), pars->serv[0]);
    // cgi.handle_cgi_request(request);
    //  std::cout << "|****************************************|" << std::endl;
    // std::cout << "all good at this place :): " << theOne.fd << std::endl;
    // std::cout << "return of send ft: " << e <<  ", len of the string: " << strlen(request.getResponse().c_str()) << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << request.Response << std::endl;
    close(theOne.fd);
    close(server.getServerFd());
    close(server.getSocket_client()[0]); 
    return (0);
}

