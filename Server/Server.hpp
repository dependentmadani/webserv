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

class Request;

static struct Client *clients = 0;

struct Client {
	private:
		int					_number_of_request;
		std::vector<int>	_fd;

	public:
		Client(): _number_of_request(0), _fd() {socket = 0;};
		~Client() {};
		void	set_fd(int val) {this->_fd.push_back(val);};
		std::vector<int>	get_fd() const {return _fd;}
		int					get_number_request() const {return _number_of_request;}

		Request *request;
		int		socket;
		socklen_t	address_length;
		struct sockaddr_storage address;
		int		received;
		struct Client *next;
};

class Server {
    public:
        Server();
        Server(int port);
        ~Server();
        
        int                 initiate_socket(int pos);
        void                accept_connections(int position);
        int                 recv_data(int position);
        struct Client       *get_client(struct Client **client, int sock);
        void                drop_client(struct Client **cl, struct Client *client);

        int                 getServerFd() const;
        std::string         getBufferString() const;
        char*               getBuffer();
        std::vector<int>    getSocket_client() const;
        int                 getSocket_fd() const;
        int                 getSocket_to_accept() const;
        int                 getFirstReadSize() const;
        int                 get_num_serv() const;
        void                setPort(int);
        void                setParse(s_parsing* );

    private:
        int                 _num_serv;
        int                 _first_read_size;
        int                 _socket_fd;
        int                 _socket_to_accept;
        struct sockaddr_in  _host_addr;
        struct addrinfo     _hints;
        int                 _port;
        std::vector<int>    _socket_client;
        char                _buffer[BUFFER_SIZE + 1];
        std::string         _buffer_complete;
        std::string         _request_hostname;
        bool                _connexion_status;
        s_parsing           *_parse;
};

#include "../Request/Request.hpp"

#endif