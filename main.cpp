/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/06/15 18:16:07 by sriyani          ###   ########.fr       */
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

// TODO: fix the blocked file descriptor in the case where it needs to complete sending of response
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
        }
        else
            return (1);
    }
    else
        exit(1);
    Server server(pars->serv[0]->ind_port);
    server.setParse(pars);
    request.ft_http_code();
    request.ft_mime_type();
    request.setParse(pars);
    fd_set rds_read, rds_read_ready, rds_write, rds_write_ready;
    FD_ZERO(&rds_read);
    FD_ZERO(&rds_read_ready);
    FD_ZERO(&rds_write);
    FD_ZERO(&rds_write_ready);
    for (int i = 0; i < pars->num_serv; ++i)
    {
        server.setPort(pars->serv[i]->ind_port);
        if (server.initiate_socket() < 0)
            continue;
        FD_SET(server.getSocket_fd(), &rds_read);
        std::cerr << "wayeeeeh " << server.getSocket_client()[i] << std::endl;
    }
    if (server.getSocket_client().empty())
    {
        std::cerr << "webserv: No available servers to work with" << std::endl;
        return 1;
    }
    int how_many_times = 0;
    int fd_size = server.getSocket_client()[server.getSocket_client().size() - 1];
    struct timeval timeout_val;
    timeout_val.tv_usec = 500000;
    int send_again = 0;
    int send_size = 0;
    while (1)
    {
        rds_read_ready = rds_read;
        rds_write_ready = rds_write;
        if (select(fd_size + 1, &rds_read_ready, &rds_write_ready, NULL, &timeout_val) < 0)
        {
            perror("select: ");
            exit(0);
        }
        std::cerr << "select function is waiting" << std::endl;
        how_many_times += 1;
        for (int i = 1; i <= fd_size; ++i)
        {
            if (FD_ISSET(i, &rds_read_ready) || FD_ISSET(i, &rds_write_ready))
            {
                if (is_available(server.getSocket_client(), i) != -1 && !request.read_again && !send_again)
                {
                    server.accept_connections(i);
                    FD_SET(server.getSocket_to_accept(), &rds_read);
                    if (server.getSocket_to_accept() > fd_size)
                        fd_size = server.getSocket_to_accept();
                    how_many_times = 0;
                    break;
                }
                else
                {
                    if (FD_ISSET(i, &rds_read_ready) && is_available(server.getSocket_client(), i) == -1 && !request.read_again)
                    {
                        if (FD_ISSET(i, &rds_write_ready) && send_again)
                        {
                            send_again = 0;
                            send_size = 0;
                            FD_CLR(i, &rds_write);
                        }
                        int checker = server.recv_data(i);
                        if (checker == -1 && is_available(server.getSocket_client(), i) == -1)
                        {
                            FD_CLR(i, &rds_read);
                            close(i);
                            continue;
                        }
                        request.setServer_index(server.get_num_serv());
                        request.setServer(server);
                        int val = request.ParseRequest(server.getBuffer());
                        request.set_read_fd(i);
                        std::cerr << "naaaaaaaaaaadiiiiiii111111" << std::endl;
                        if (val == 1)
                        {
                            if (is_available(server.getSocket_client(), i) == -1)
                            {
                                FD_CLR(i, &rds_read);
                                close(i);
                                how_many_times = 0;
                            }
                            continue;
                        }
                        else if (val == 0)
                            request.UseMethod();
                        if (request.read_again)
                            continue;
                        std::cerr << "naaaaaaaaaaadiiiiiii222222" << std::endl;
                        request.build_response();
                        FD_SET(i, &rds_write);
                        send_again = 1;
                        break;
                    }
                    else if ((request.read_again || send_again))
                    {
                        if (!send_again && request.read_again && FD_ISSET(i, &rds_read_ready))
                        {
                            if (request.UseMethod())
                                continue;
                            request.build_response();
                            FD_SET(i, &rds_write);
                            send_again = 1;
                        }
                        else if (send_again && FD_ISSET(i, &rds_write_ready))
                        {
                            int ret = 0;
                            ret = send(i, &(request.Response.c_str())[send_size], request.Response.size() - send_size, 0);
                            if (ret > 0)
                            {
                                send_size += ret;
                            }
                            if (send_size < (int)request.Response.size() && ret > 0)
                                continue;
                            else
                            {
                                FD_CLR(i, &rds_read);
                                FD_CLR(i, &rds_write);
                                std::cerr << "close 3 : " << i << std::endl;
                                close(i);
                                send_again = 0;
                                send_size = 0;
                            }
                        }
                        else if (send_again && request.read_again)
                        {
                            send_again = 0;
                            request.read_again = 0;
                        }
                        how_many_times = 0;
                    }
                    else if (is_available(server.getSocket_client(), i) == -1)
                    {
                        close(i);
                        FD_CLR(i, &rds_read);
                        FD_CLR(i, &rds_write);
                        how_many_times = 0;
                    }
                }
            }
        }
    }
    return (0);
}
