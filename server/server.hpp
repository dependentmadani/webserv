/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:28:06 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/24 20:28:08 by mbadaoui         ###   ########.fr       */
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
