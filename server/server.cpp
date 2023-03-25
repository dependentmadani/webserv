/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:09:05 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/25 12:09:05 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server() : _host_addr(), _socket_client()
{
    _socket_fd = 0;
    _port = 8080;
}

Server::Server(int port) : _host_addr(), _socket_client()
{
    _socket_fd = 0;
    _port = port;
}

Server::~Server()
{
    std::cout << "destructor used" << std::endl;
}

int Server::initiat_server() {
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM is virtual circuit service, and AF_INET is IP
    if (_socket_fd < 0) {
        perror("webserv error (socket) ");
        return -1;
    }
    std::cout << "The server created successfully, with fd value of " << _socket_fd << std::endl;

    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = htons(_port);
    _host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    int i = bind(_socket_fd, (struct sockaddr *)&_host_addr, sizeof(_host_addr));
    if (i < 0) {
        perror("webserv error (bind) ");
        return -1;
    }
    std::cout << "Now, we are going to listen, for requests" << std::endl;
    if (listen(_socket_fd, 5))
    {
        perror("webserv error (listen) ");
        return -1;
    }
    std::cout << "listen works successfully" << std::endl;
    return 0;
}

void    Server::accept_connections()
{
    while (1)
    {
        int addr_length = sizeof(_host_addr);
        int socket_to_accept = accept(_socket_fd, (struct sockaddr*)&_host_addr, (socklen_t*)&addr_length);
        if (socket_to_accept < 0)
        {
            perror("webserv error (accept)");
            exit(EXIT_FAILURE);
        }
        std::cout << "working properly" << std::endl;
    }
}

// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

// /* Internet address. */
// struct in_addr {
//    uint32_t       s_addr;     /* address in network byte order */
// };
