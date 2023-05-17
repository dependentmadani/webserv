/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:09:12 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/17 16:57:58 by sriyani          ###   ########.fr       */
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

# define BUFFER_SIZE 102400

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
        void                setPort(int);
        void                setParse(s_parsing* );

    private:
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
