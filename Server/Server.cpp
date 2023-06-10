/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:09:05 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/27 16:30:27 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server() : _first_read_size(), _host_addr(), _socket_client(), _buffer_complete(), _request_hostname()
{
    _num_serv = 0;
    _socket_fd = 0;
    _socket_to_accept = 0;
    _port = 8080;
    _connexion_status = false;
}

Server::Server(int port) : _first_read_size(), _host_addr(), _socket_client(), _buffer_complete()
{
    _num_serv = 0;
    _socket_fd = 0;
    _socket_to_accept = 0;
    _port = port;
    _connexion_status = false;
}

Server::~Server()
{
    std::cout << "destructor used" << std::endl;
}

int Server::initiate_socket()
{
    // int opt = 1;
    // struct addrinfo hints;
    // struct addrinfo *bind_address;

    // memset(&hints, 0, sizeof(hints));
    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = htons(_port);
    _host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // hints.ai_family = AF_INET;
    // hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;

    // std::cerr << "the host would be: " << _parse->serv[num_serv]->host.c_str() << std::endl;
    // std::cerr << "server name: |" << _parse->serv[num_serv]->server_name.c_str() << "|...." << std::endl;
    // int g = getaddrinfo(_parse->serv[num_serv]->host.c_str(), std::to_string(_port).c_str(), &hints, &bind_address);

    // std::cerr << "the return value of getaddinfo: " << g << std::endl;
    // _socket_fd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol); // SOCK_STREAM is virtual circuit service, and AF_INET is IP
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0)
    {
        perror("webserv error (socket) ");
        return -1;
    }
    const int on = 1;
    // if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
    // {
    //     std::cerr << "Failed to set socket option" << std::endl;
    //     return 1;
    // }
    setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    // if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
    // {
    //     perror("webserv error (setsockop) ");
    //     return -1;
    // }
    std::cout << "The server created successfully, with fd value of " << _socket_fd << std::endl;
    std::cout << "The port to listen to: " << _port << std::endl;
    std::cout << "Binding the socket " << _socket_fd << std::endl;
    // int i = bind(_socket_fd, bind_address->ai_addr, bind_address->ai_addrlen);
    // const int enable = 1;
    // setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    // int i = bind(_socket_fd, bind_address->ai_addr, bind_address->ai_addrlen);
    int i = bind(_socket_fd, (struct sockaddr *)&_host_addr, sizeof(_host_addr));
    if (i < 0) {
        // freeaddrinfo(bind_address);
        std::cerr << "error: " << errno << ", " << strerror(errno) << std::endl;
        // perror("webserv error (bind) ");
        return -1;
    }
    // freeaddrinfo(bind_address);
    _socket_client.push_back(_socket_fd);
    std::cout << "Now, we are going to listen, for requests" << std::endl;
    if (listen(_socket_fd, 10) < 0)
    {
        perror("webserv error (listen) ");
        return -1;
    }
    std::cout << "listen works successfully" << std::endl;
    return 0;
}

void Server::accept_connections(int position)
{
    int addr_length = sizeof(_host_addr);

    _socket_to_accept = accept(position, (struct sockaddr *)&_host_addr, (socklen_t *)&addr_length);
    if (_socket_to_accept < 0)
    {
        perror("webserv error (accept)");
        _socket_to_accept = -1;
    }
    // fcntl(_socket_to_accept,F_SETFL,O_NONBLOCK);
    // std::cout << "working properly" << std::endl;
    // char buffer[1024] = {0};
    // int valread = read( socket_to_accept , buffer, 1024);
    // std::cout << buffer << "\n" << std::endl;
    // if(valread < 0)
    // {
    //     printf("No bytes are there to read");
    // }
    // char hello[78] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";//IMPORTANT! WE WILL GET TO IT
    // write(socket_to_accept , hello , sizeof(hello));
    // close(socket_to_accept);
    // std::cout << "\n listening to new socket \n" << std::endl;
    // close(_socket_to_accept);
}

int Server::recv_data(int position)
{
    std::ofstream file;
    file.open("temp_file");
    memset(_buffer, 0, BUFFER_SIZE);

    // while (data > 0) {
    _first_read_size = recv(position, _buffer, BUFFER_SIZE, 0);
    for (int i = 0; i < _first_read_size; ++i) {
        file << _buffer[i];
    }
    // int i = 0;
    // while(1)
    // {
    //      _first_read_size = recv(position, _buffer, BUFFER_SIZE, 0);
    //      if(_first_read_size == 0)
    //         break;
    //      std::cout << _first_read_size<< " i " << i << std::endl;
    //      i++;
    // }
    // exit(1);
    _buffer_complete.append(std::string(_buffer));
    std::cout << "check what: " << position << std::endl;
    // for (int i = 0; i < 8000; ++i) {
    //     std::cerr << _buffer[i];
    // }
    // }
    size_t find_host = _buffer_complete.find("Host:");
    size_t find_next_cr = 0;
    // std::cerr << "check this: |" << _buffer_complete.substr(find_host , _buffer_complete.size()) << "|" << std::endl;
    if (find_host != std::string::npos)
    {
        find_next_cr = _buffer_complete.substr(find_host , _buffer_complete.size()).find("\r\n");
        _request_hostname =  _buffer_complete.substr(find_host + std::string("Host: ").size() , find_next_cr - std::string("Host: ").size());
    }
    int which_serv = -1;
    for (int i = 0; i < _parse->num_serv; ++i) {
        if (_parse->serv[i]->server_name == _request_hostname) {
            std::cout << "it does match the servername :)" << std::endl;
            which_serv = i;
            break ;
        }
        else if ((_parse->serv[i]->host + ":" + std::to_string(_parse->serv[i]->ind_port)) == _request_hostname) {
            std::cout << "it does not match the host :))))))) " << i << std::endl;
            which_serv = i;
            break ;
        }
        std::cerr << "hihihihi: |" << ((_parse->serv[i]->host + ": " + std::to_string(_parse->serv[i]->ind_port))) << "|" << std::endl;
        std::cerr << "hohohoho: |" << _parse->serv[i]->server_name << "|"<< std::endl;
        std::cerr << "hahahaha: |" << _request_hostname << "|" << std::endl;
    }
    if (which_serv == -1) {
        which_serv = 0;
    }
    _num_serv = which_serv;
    // std::cerr << "|" << _buffer_complete[find_next_cr - 2] << "|" << std::endl;
    // std::cerr << "the position of host: " << find_host << ", the position of cr: " << find_next_cr << std::endl;
    // if (data < 0)
    // {
    // 	_connexion_status = true;
    // 	std::cout << "webserv error (recv)" << std::endl;
    // 	return (data);
    // }
    // if (data == 0)
    // {
    // 	std::cout << "connection closed from remote side" << std::endl;
    // 	_connexion_status = true;
    // 	return(data);
    // }
    // if (data == 0)
    // {
    // 	std::cout << "connection closed from remote side" << std::endl;
    // 	_connexion_status = true;
    // 	return(data);
    // }
    _buffer_complete.append("\0");
    // for (int i =0; i < data; ++i) {
    //     std::cerr << _buffer_complete[i];
    // }
    std::cout << "\n\n" << std::endl;
	std::cout << "\n\n" << "===============   "  << _first_read_size << " BYTES  RECEIVED   ===============\n";
	// std::cout << _buffer;
	std::cout << "\n======================================================" << std::endl;
	return (_num_serv);
}

int Server::getServerFd() const
{
    return _socket_fd;
}

char *Server::getBuffer()
{
    return this->_buffer;
}

std::string Server::getBufferString() const
{
    return this->_buffer_complete;
}

std::vector<int> Server::getSocket_client() const
{
    return this->_socket_client;
}

void Server::setParse(s_parsing *parsed)
{
    this->_parse = parsed;
}

int Server::getSocket_fd() const
{
    return _socket_fd;
}

int Server::getFirstReadSize() const {
    return _first_read_size;
}

void    Server::setPort(int port) {
    _port = port;
}

int Server::getSocket_to_accept() const
{
    return this->_socket_to_accept;
}

std::string Server::get_request_hostname() const {
    return this->_request_hostname;
}

void    Server::set_num_serv(int num_serv) {
    _num_serv = num_serv;
}
