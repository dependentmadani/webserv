/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/21 18:01:20 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "Server/Server.hpp"
#include "Request/Request.hpp"
#include "parse/parsing.hpp"
#include "CGI/cgi.hpp"

int is_available(std::vector<int> tmp, int value)
{

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        if (tmp[i] == value)
            return i;
    }
    return -1;
}

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
            std::cout << " File exist" << std::endl;
        }
        else
        {
            std::cout << " File doesn't exist" << std::endl;
            return (1);
        }
    }
    else
    {
        std::cout << "Error from number of arguments, make sure to have something as follows \"./webserv config_file.conf\"" << std::endl;
        exit(1);
    }
    Server server(pars->serv[0]->ind_port);
    // struct pollfd theOne;
    // memset(&theOne, 0, sizeof(theOne));
    server.setParse(pars);
    request.ft_http_code();
    request.ft_mime_type();
    request.setParse(pars);
    fd_set rds, rds_ready;
    FD_ZERO(&rds);
    FD_ZERO(&rds_ready);
    for (int i = 0; i < pars->num_serv; ++i)
    {
        server.setPort(pars->serv[i]->ind_port);
        if (server.initiate_socket() < 0)
            continue ;
        FD_SET(server.getSocket_fd(), &rds);
        std::cerr << "wayeeeeh " << server.getSocket_client()[i] << std::endl;
    }
    int how_many_times = 0;
    while (1)
    {
        rds_ready = rds;
        if (select(FD_SETSIZE, &rds_ready, NULL, NULL, NULL) < 0)
        {
            perror("select: ");
            exit(0);
        }
        how_many_times += 1;
        int accepted_connection = 0;
        for (int i = 1; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, & rds_ready))
            {
                int server_id = 0;
                if ((server_id = is_available(server.getSocket_client(), i)) != -1 && !request.read_again)
                {
                    // server.set_num_serv(server_id);
                    // request.setServer_index(server_id);
                    std::cerr << "accept a connection " << i <<std::endl;
                    server.accept_connections(i);
                    accepted_connection = i;
                    FD_SET(server.getSocket_to_accept(), &rds);
                }
                else
                {
                std::cerr << "how_many_times: " << how_many_times << std::endl;
                if (request.read_again)
                {
                    std::cerr << "That woouuuuld be cooool " << std::endl;
                    if (request.UseMethod())
                        continue;
                    request.build_response();
                    // std::cerr << "it diiiid reaaach heree: " << server.getFirstReadSize() << std::endl;
                    send(i , request.Response.c_str(), strlen(request.Response.c_str()), 0);
                    how_many_times = 0;
                    std::cerr << "************------******************" << std::endl;
                    std::cerr << "all should be good :):):)" << std::endl;
                    // std::cout << request.Response << std::endl;
                    // std::cerr << server.getBuffer() << std::endl;
                    FD_CLR(i , &rds);
                    close(i);
                }
                else if (!request.read_again && how_many_times == pars->num_serv)
                {
                    std::cerr << "wooow waaas heeere" << std::endl;
                    int d = server.recv_data(i);
                    std::cerr << "the server index would be: " << d << std::endl;
                    request.setServer_index(d);
                    request.setServer(server);
                    std::cerr << server.getBuffer() << std::endl;
                    int val = request.ParseRequest(server.getBuffer());
                    if (val == 1)
                    {
                        close(i);
                        FD_CLR(i , &rds);
                        break ;
                    }
                    else if (val == 0){
                    request.UseMethod();
                    }
                    request.set_read_fd(i);
                    if (request.read_again)
                        continue ;
                    request.build_response();
                    // std::cerr << "it diiiid reaaach heree: " << server.getFirstReadSize() << std::endl;
                    std::cerr << "hooooolaaaallaaaaa: " << request.Response.size() << " and " <<  request.getFile_size() <<std::endl;
                    // send(i , request.Response.c_str(), BUFFER_SIZE, 0);
                    // send(i , request.Response.c_str(), strlen(request.Response.c_str()) + request.getFile_size(), 0);
                    send(i , request.Response.c_str(), request.Response.size() , 0);
                    how_many_times = 0;
                    std::cerr << "*********************************************" << std::endl;
                    // std::cout << request.Response << std::endl;
                    // std::cerr << "all should be good :)" << std::endl;
                    // std::cerr << server.getBuffer() << std::endl;
                    close(i);
                    FD_CLR(i , &rds);
                }
                else {
                    close(i);
                    FD_CLR(i , &rds);
                }
            }
                // else {
                //     std::cerr << "hooooolaaaallaaaa" <<std::endl;
                //     request.build_response();
                //     // std::cerr << "it diiiid reaaach heree: " << server.getFirstReadSize() << std::endl;
                //     send(i , request.Response.c_str(), strlen(request.Response.c_str()), 0);
                //     how_many_times = 0;
                //     std::cerr << "************------******************" << std::endl;
                //     // std::cout << request.Response << std::endl;
                //     std::cerr << "all should be good :):):)" << std::endl;
                //     // std::cerr << server.getBuffer() << std::endl;
                //     FD_CLR(i , &rds);
                //     close(i);
                // }
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
