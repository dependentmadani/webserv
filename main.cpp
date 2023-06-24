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
#include <signal.h>
#include "Server/Server.hpp"
#include "Request/Request.hpp"
#include "parse/parsing.hpp"
#include "CGI/cgi.hpp"

std::string    cgi_helper_function(Request &req) {
    int status = 0;
    int state = 1;
    int check = waitpid(req.get_cgi_child_process(), &status, WNOHANG);
    if ( check == -1) {
        perror("wait() error");
        state = 0;
    }
    if (check == 0)
    {
        state = 0;
        return "";
    }
    if (check == req.get_cgi_child_process())
        state = 1;
    if (state != 1)
        return "";
    int rd;
    std::string response;
    std::string hold_ContentType;
    char bufffer[4096] = " ";
    int out_fd = open("./out_result.txt", O_RDONLY);
    while ((rd = read(out_fd, bufffer, sizeof(bufffer))) > 0)
    {
        response += bufffer;
    }
    close(out_fd);
    unlink("file.txt");
    unlink("out_result.txt");
    size_t found = 0;
    if (req.get_cgi_ext() == ".pl")
        found = response.find("\n\n");
    else
        found = response.find("\r\n\r\n");
    hold_ContentType = response.substr(0, found);
    size_t fnd = hold_ContentType.find("Content-type:");
    if (fnd != std::string::npos)
        response = response.substr(found + 1);
    if (response.empty()) {
        response = "<!DOCTYPE html>"
                            "<html>"
                            "<body>"
                            "</body>"
                            "</html>";
    }
    return response;
}

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
    fd_set rds_read, rds_read_ready, rds_write, rds_write_ready;
    FD_ZERO(&rds_read);
    FD_ZERO(&rds_read_ready);
    FD_ZERO(&rds_write);
    FD_ZERO(&rds_write_ready);
    for (int i = 0; i < pars->num_serv; ++i)
    {
        server.setPort(pars->serv[i]->ind_port);
        if (server.initiate_socket(i) < 0)
            continue;
        FD_SET(server.getSocket_fd(), &rds_read);
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
                if (is_available(server.getSocket_client(), i) != -1)
                {
                    struct Client *client = server.get_client(&clients, -1);
                    server.accept_connections(i);
                    client->socket = server.getSocket_to_accept();
                    FD_SET(server.getSocket_to_accept(), &rds_read);
                    if (server.getSocket_to_accept() > fd_size)
                        fd_size = server.getSocket_to_accept();
                    how_many_times = 0;
                    break;
                }
                struct Client *client = clients;
                while (client) {
                    struct Client *next = client->next;
                    if (i == client->socket && (FD_ISSET(client->socket, &rds_read_ready) || FD_ISSET(client->socket, &rds_write_ready))) {
                        if (FD_ISSET(client->socket, &rds_read_ready)  && !client->request->read_again && !client->request->get_cgi_helper())
                        {
                            if (FD_ISSET(client->socket, &rds_write_ready) && FD_ISSET(client->socket, &rds_read_ready))
                            {
                                client->request->send_again = 0;
                                client->request->send_size = 0;
                                FD_CLR(client->socket, &rds_write);
                                break ;
                            }
                            int checker = server.recv_data(client->socket);
                            if (checker == -1 && is_available(server.getSocket_client(), client->socket) == -1)
                            {
                                FD_CLR(client->socket, &rds_read);
                                close(client->socket);
                                server.drop_client(&clients, client);
                                break;
                            }
                            client->request->setServer_index(server.get_num_serv());
                            client->request->setServer(server);
                            int val = client->request->ParseRequest(server.getBuffer());
                            if (val == 1)
                            {
                                if (is_available(server.getSocket_client(), client->socket) == -1)
                                {
                                    FD_CLR(client->socket, &rds_read);
                                    close(client->socket);
                                    server.drop_client(&clients, client);
                                    how_many_times = 0;
                                    break ;
                                }
                                break;
                            }
                            else if (val == 0) {
                                client->request->set_read_fd(client->socket);
                                client->request->UseMethod();
                            }
                            if (client->request->read_again)
                                break;
                            if (client->request->get_cgi_helper()) {
                                FD_SET(client->socket, &rds_write);
                                break;
                            }
                            client->request->build_response();
                            FD_SET(client->socket, &rds_write);
                            client->request->send_again = 1;
                            break;
                        }
                        else if ((client->request->read_again || client->request->send_again || !client->request->finished || client->request->get_cgi_helper()))
                        {
                            if (FD_ISSET(client->socket, &rds_write_ready) && FD_ISSET(client->socket, &rds_read_ready))
                            {
                                client->request->send_again = 0;
                                client->request->send_size = 0;
                                client->request->set_cgi_helper(0);
                                kill(client->request->get_cgi_child_process(), SIGKILL);
                                FD_CLR(client->socket, &rds_write);
                                break ;
                            }
                            if (!client->request->send_again && client->request->read_again && FD_ISSET(client->socket, &rds_read_ready))
                            {
                                if (client->request->UseMethod())
                                    break;
                                client->request->build_response();
                                FD_SET(client->socket, &rds_write);
                                client->request->send_again = 1;
                            }
                            else if ((client->request->send_again || !client->request->finished) && !client->request->get_cgi_helper() && FD_ISSET(i, &rds_write_ready))
                            {
                                int ret = 0;
                                ret = send(client->socket, &(client->request->Response.c_str())[client->request->send_size], client->request->Response.size() - client->request->send_size, 0);
                                if (ret > 0)
                                    client->request->send_size += ret;
                                else {
                                    client->request->send_size = 0;
                                    client->request->send_again = 0;
                                }
                                if (client->request->send_size < (int)client->request->Response.size() && ret > 0)
                                    break;
                                else if (client->request->finished && client->request->get_cgi_helper() && ret <= 0)
                                {
                                    FD_CLR(client->socket, &rds_read);
                                    FD_CLR(client->socket, &rds_write);
                                    close(client->socket);
                                    server.drop_client(&clients, client);
                                    break;
                                }
                                else
                                {
                                    client->request->send_size = 0;
                                    client->request->send_again = 0;
                                }
                                if (!client->request->finished)
                                    client->request->UseMethod();
                            }
                            else if (client->request->get_cgi_helper())
                            {
                                std::string hada = cgi_helper_function(*client->request);
                                if (hada == "")
                                    break;
                                client->request->set_cgi_helper(0);
                                client->request->ft_http_status(200);
                                client->request->set_response_as_body(hada);
                                client->request->set_content_type("html");
                                client->request->set_content_length(hada.size());
                                client->request->build_response();
                                client->request->send_again = 1;
                                break;
                            }
                            else if (client->request->send_again && client->request->read_again)
                            {
                                client->request->send_again = 0;
                                client->request->read_again = 0;
                            }
                            how_many_times = 0;
                        }
                        else if (is_available(server.getSocket_client(), client->socket) == -1 && client->request->finished && !client->request->get_cgi_helper())
                        {
                            FD_CLR(client->socket, &rds_read);
                            FD_CLR(client->socket, &rds_write);
                            close(client->socket);
                            server.drop_client(&clients, client);
                            break;
                            how_many_times = 0;
                        }
                    }
                    client = next;
                }
            }
        }
    }
    return (0);
}
