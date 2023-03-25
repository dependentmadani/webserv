/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:09:12 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/25 12:09:13 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <errno.h>
# include <vector>

class Server {
    public:
        Server();
        Server(int port);
        ~Server();
        
        int     initiat_server();
        void    accept_connections();

    private:
        int                 _socket_fd;
        struct sockaddr_in  _host_addr;
        int                 _port;
        std::vector<int>    _socket_client;
};

#endif
