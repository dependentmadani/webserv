/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:09:12 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/21 13:50:40 by mbadaoui         ###   ########.fr       */
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
# include <fcntl.h>
# include <errno.h>
# include <netdb.h>
# include "../parse/parsing.hpp"

# define BUFFER_SIZE 180000

class Server {
    public:
        Server();
        Server(int port);
        ~Server();
        
        int     initiate_socket(int num_serv);
        void    accept_connections(int position);
        int     recv_data(int position);

        int                 getServerFd() const;
        std::string         getBufferString() const;
        char*               getBuffer();
        std::vector<int>    getSocket_client() const;
        int                 getSocket_fd() const;
        int                 getSocket_to_accept() const;
        int                 getFirstReadSize() const;
        void                setPort(int);
        void                setParse(s_parsing* );

    private:
        int                 _first_read_size;
        int                 _socket_fd;
        int                 _socket_to_accept;
        struct sockaddr_in  _host_addr;
        int                 _port;
        std::vector<int>    _socket_client;
        char                _buffer[BUFFER_SIZE + 1];
        std::string         _buffer_complete;
        bool                _connexion_status;
        fd_set              _readfds;
        s_parsing           *_parse;
};

#endif