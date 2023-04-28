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
# include <poll.h>
# include <stdio.h>
# include <cstring>
# include <errno.h>
# include <unistd.h>
# include <vector>
# include <errno.h>
# include "../parse/parsing.hpp"

# define BUFFER_SIZE 8192

class Server {
    public:
        Server();
        Server(int port);
        ~Server();
        
        int     initiat_server();
        void    accept_connections();
        int     recv_data(struct pollfd *poll);

        int                 getServerFd() const;
        char               *getBuffer();
        std::vector<int>    getSocket_client() const;
        int                 getSocket_fd() const;
        void                setParse(s_parsing* );

    private:
        int                 _socket_fd;
        int                 _socket_to_accept;
        struct sockaddr_in  _host_addr;
        int                 _port;
        std::vector<int>    _socket_client;
        char                _buffer[BUFFER_SIZE + 1];
        bool                _connexion_status;
        s_parsing           *_parse;
};

#endif
