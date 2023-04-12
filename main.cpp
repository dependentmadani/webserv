/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/12 14:27:49 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "Server/Server.hpp"
#include "Request/Request.hpp"
#include "parse/parsing.hpp"

int main(int ac, char **av)
{
    std::ifstream file;
    Request request;
    parsing vars;
    parsing *pars = new parsing();
    
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
            // return (1);
        }
    }
    else
        std::cout<<"Error from number of arguments, make sure to have something as follows \"./webserv config_file.conf\""<<std::endl;
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
    close(theOne.fd);
    close(server.getServerFd());
    request.setParse(pars);
    request.ParseRequest(server.getBuffer());

    request.UseMethod();
    // request.is_location_has_cgi();
    return (0);
}