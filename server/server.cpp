/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:28:01 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/24 20:28:02 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iostream>

// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

// /* Internet address. */
// struct in_addr {
//    uint32_t       s_addr;     /* address in network byte order */
// };

#define PORT 8080

int Server::initiat_server() {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM is virtual circuit service, and AF_INET is IP
    if (socketfd < 0){
        perror("WEBSERV (failed): ");
        return -1;
    }
    std::cout << "The server created successfully, with fd value of " << socketfd << std::endl;
    struct sockaddr_in host_addr;

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    int i = bind(socketfd, (struct sockaddr *)&host_addr, sizeof(host_addr));
    if (i != 0) {
        perror("webserver (bind)");
        return -1;
    }
    printf("socket successfully bound to address, and the value is {%d}\n", i);
    return 0;
}

void    Server::accept_connections()
{
    
}