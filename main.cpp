/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/28 11:24:49 by sriyani          ###   ########.fr       */
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
//TODO: fix the blocked file descriptor in the case where it needs to complete sending of response
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
            //std::cout << " File exist" << std::endl;
        }
        else
        {
            //std::cout << " File doesn't exist" << std::endl;
            return (1);
        }
    }
    else
    {
        //std::cout << "Error from number of arguments, make sure to have something as follows \"./webserv config_file.conf\"" << std::endl;
        exit(1);
    }
    Server server(pars->serv[0]->ind_port);
    // struct pollfd theOne;
    // memset(&theOne, 0, sizeof(theOne));
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
    int how_many_times = 0;
    int fd_size = server.getSocket_client()[server.getSocket_client().size() - 1];
    // for (size_t i = 0; i < server.getSocket_client().size(); ++i){
    //     std::cout << "the values: "<< server.getSocket_client()[i] << " " << server.getSocket_client().size() << std::endl;
    // }
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
        // int accepted_connection = 0;
        int server_id = 0;
        for (int i = 1; i <= fd_size; ++i)
        {
            std::cerr << "check the return value of read: " << FD_ISSET(i, &rds_read_ready) << ", and write: " << FD_ISSET(i, &rds_write_ready) << ", at: " << i << std::endl;
            if (FD_ISSET(i, &rds_read_ready) || FD_ISSET(i, &rds_write_ready))
            {
                // std::cerr << "khassso ikouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuun hna" << std::endl;
                // std::cout << "pass through select: "<< i << std::endl;
                if ( (server_id = is_available(server.getSocket_client(), i)) != -1 && !request.read_again && !send_again )
                {
                    // request.setServer_index(server_id);
                    std::cerr << "accept a connection " << i << std::endl;
                    server.accept_connections(i);
                    // accepted_connection = i;
                    FD_SET(server.getSocket_to_accept(), &rds_read);
                    // std::cerr << "the value of fd_size before: " << fd_size << std::endl;
                    if (server.getSocket_to_accept() > fd_size)
                        fd_size = server.getSocket_to_accept();
                    how_many_times = 0;
                    // std::cerr << "the value of fd_size: " << fd_size << std::endl;
                }
                else
                {
                    std::cerr << "how_many_times: " << how_many_times << ",send_again: " << send_again << ",request.read_again: " << request.read_again << std::endl;
                    // std::cerr << "read again value: " << request.read_again << std::endl;
                    //the first time, and first entry should be in this function
                    if (FD_ISSET(i, &rds_read_ready))
                    {
                        // std::cerr << "wooow waaas heeere" << std::endl;
                        std::cerr << "That woouuuuld be cooool to reeaaaaaad" << std::endl;
                        int checker = server.recv_data(i);
                        if (checker == -1 && is_available(server.getSocket_client(), i) == -1)
                        {
                            FD_CLR(i, &rds_read);
                            std::cerr << "close 1 : " << i << std::endl;
                            close(i);
                            continue ;
                        }
                        request.setServer_index(server.get_num_serv());
                        request.setServer(server);
                        //std::cerr << server.getBuffer() << std::endl;
                        int val = request.ParseRequest(server.getBuffer());
                        request.set_read_fd(i);
                        if (val == 1)
                        {
                            std::cerr << "close 2 : " << i << std::endl;
                            if (is_available(server.getSocket_client(), i) == -1) {
                                FD_CLR(i, &rds_read);
                                close(i);
                                how_many_times = 0;
                            }
                            // else {
                            //     FD_CLR(i, &rds_read);
                            //     FD_CLR(i, &rds_write);
                            // }
                            continue ;
                        }
                        else if (val == 0)
                            request.UseMethod();
                        if (request.read_again)
                            continue;
                        request.build_response();
                        std::cerr << "normalement dakchi nadi" << std::endl;
                        FD_SET(i, &rds_write);
                        send_again = 1;
                        FD_CLR(i, &rds_read);
                    }
                    // if there is remaining stuff to read or to send, it should be here
                    else if ( (request.read_again || send_again) )
                    {
                        char buffer[BUFFER_SIZE];
                        memset(buffer, 0, BUFFER_SIZE);

                        int d = read(i, buffer, BUFFER_SIZE);
                        std::cerr << "the read return value: " << d << std::endl;
                        std::cerr << "That woouuuuld be cooool to wriiite" << std::endl;
                        if (!send_again && request.read_again && FD_ISSET(i, &rds_read_ready)) {
                            if (request.UseMethod()) 
                                continue ;
                            request.build_response();
                            FD_SET(i, &rds_write);
                            send_again = 1;
                            FD_CLR(i, &rds_read);
                        }
                        else if ( send_again && FD_ISSET(i, &rds_write_ready) ) {
                            std::cerr << "----------------------------was heree----------------------------fd: " << i << std::endl;
                            int ret = 0;
                            if ((ret = send(i, &(request.Response.c_str())[send_size], request.Response.size() - send_size, 0)) <= 0) {
                                std::cerr << "should not be here at alll aaaaaa hhaaaaamiiiiidd" << std::endl;
                                FD_CLR(i, &rds_write);
                                std::cerr << "close 3 : " << i << std::endl;
                                close(i);
                                send_again = 0;
                                send_size = 0;
                                std::cerr << "cloosed successefully!!!!!!!!!!!!!" << std::endl;
                                continue ;
                            }
                            std::cerr << "the return value of send: " << ret << std::endl;
                            // std::cerr << "the value of returned value of send: " << ret << std::endl;
                            if (ret > 0) {
                                send_size += ret;
                            }
                            std::cerr << "send_size: " << send_size << ", request.Response size: " << request.Response.size() << std::endl;
                            if (send_size < (int)request.Response.size() && ret > 0) {
                                std::cerr << "******keep seeendiiiiing*****fd: " << i << std::endl;
                                continue ;
                            }
                        }
                        how_many_times = 0;
                        std::cerr << "************------******************" << std::endl;
                    }
                    else if (is_available(server.getSocket_client(), i) == -1){
                        std::cerr << "*************************was here************************************" << std::endl;
                        std::cerr << "close 4 : " << i << std::endl;
                        close(i);
                        FD_CLR(i, &rds_read);
                        FD_CLR(i, &rds_write);
                        how_many_times = 0;
                    }
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
        // //std::cerr << "*********************************************" << std::endl;
        // // for (int i = 0; i < n; ++i) {
        // //     //std::cerr << request.getBody()[i];
        // // }
        // //std::cout << "----------------------------------" << std::endl;
        // //std::cout << request.Response << std::endl;
        // // close(theOne.fd);
        // close(server.getServerFd());
        // close(server.getSocket_client()[0]);

        // system("leaks webserv");
    }
    return (0);
}
