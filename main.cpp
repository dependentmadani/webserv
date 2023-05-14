/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/11 10:57:24 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "Server/Server.hpp"
#include "Request/Request.hpp"
#include "parse/parsing.hpp"
#include "cgi-bin/cgi.hpp"

int is_available(std::vector<int> tmp, int value) {

    for (size_t i =0 ; i < tmp.size(); ++i) {
        if (tmp[i] == value)
            return 1;
    }
    return 0;
}

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
    // struct pollfd theOne;
    // memset(&theOne, 0, sizeof(theOne));
    server.setParse(pars);
    request.ft_http_code();
    request.ft_mime_type();
    request.setParse(pars);
    fd_set  rds, rds_ready;
    FD_ZERO(&rds);
    FD_ZERO(&rds_ready);
    for (int i = 0; i < pars->num_serv; ++i) {
    server.setPort(pars->serv[i]->ind_port);
    if (server.initiate_socket(i) < 0)
        return (1);
    FD_SET(server.getSocket_fd(), &rds);
    std::cerr << "wayeeeeh " << server.getSocket_client()[i] << std::endl;
    }
    while (1) {
    rds_ready = rds;
    if (select(FD_SETSIZE, &rds_ready, NULL, NULL, NULL) < 0) {
        perror("select: ");
        exit(0);
    }
    int accepted_connection = 0;
    for (int i = 1; i < FD_SETSIZE; ++i) {
        if (FD_ISSET(i, & rds_ready)) {
            if (is_available(server.getSocket_client(), i)) {
                std::cerr << "accept a connection " << i <<std::endl;
                server.accept_connections(i);
                accepted_connection = i;
                FD_SET(server.getSocket_to_accept(), &rds);
            }
            else {
                server.recv_data(i);
                // std::cerr << "it diiiid reaaach heree" << std::endl;
                std::cerr << server.getBuffer() << std::endl;
                request.ParseRequest(server.getBuffer());
                request.UseMethod();
                request.build_response();
                send(i , request.Response.c_str(), strlen(request.Response.c_str()), 0);
                std::cerr << "*********************************************" << std::endl;
                std::cout << request.Response << std::endl;
                FD_CLR(i , &rds);
                std::cerr << "all should be good :)" << std::endl;
                close(i);
            }
        }
    }
    // server.accept_connections();
    // theOne.fd = (server.getSocket_client())[0];
    // theOne.events = POLLIN;
    // int n = server.recv_data(&theOne);
    // request.ft_http_code();
    // request.ft_mime_type();
    // request.setParse(pars);
    // request.ParseRequest(server.getBuffer());
    // request.UseMethod();
    // request.build_response();
    // send(server.getSocket_client()[0] , request.Response.c_str(), strlen(request.Response.c_str()), 0);
    // std::cerr << "*********************************************" << std::endl;
    // // for (int i = 0; i < n; ++i) {
    // //     std::cerr << request.getBody()[i];
    // // }
    // std::cout << "----------------------------------" << std::endl;
    // std::cout << request.Response << std::endl;
    // // close(theOne.fd);
    // close(server.getServerFd());
    // close(server.getSocket_client()[0]); 
    }
    return (0);
}

