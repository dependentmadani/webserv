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

Server::Server(int port) : _first_read_size(), _host_addr(), _socket_client(), _buffer_complete(), _request_hostname()
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
    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = htons(_port);
    _host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0)
    {
        perror("webserv error (socket) ");
        return -1;
    }
    int return_fd = fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
    if (return_fd < 0){
        perror("webserv error1 (fcntl)");
        exit(1);
    }
    const int on = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0) {
        std::cerr << "Failed to set socket option" << std::endl;
    }
    int i = bind(_socket_fd, (struct sockaddr *)&_host_addr, sizeof(_host_addr));
    if (i < 0) {
        perror("webserv error (bind) ");
        return -1;
    }
    _socket_client.push_back(_socket_fd);
    std::cout << "Now, we are going to listen, for requests" << std::endl;
    if (listen(_socket_fd, SOMAXCONN) < 0)
    {
        std::cerr << "webserv error port: " << _port << " ";
        perror("(listen) ");
        return -1;
    }
    std::cout << "listen successfully on this port: " << _port << std::endl;
    return 0;
}

void Server::accept_connections(int position)
{
    int addr_length = sizeof(_host_addr);

    _socket_to_accept = accept(position, (struct sockaddr *)&_host_addr, (socklen_t *)&addr_length);
    std::cerr << "fd of accepted socket before is: " << _socket_to_accept << " position: " << position << std::endl;
    if (_socket_to_accept < 0)
    {
        perror("webserv error (accept)");
        exit(1);
    }
    int return_fd = fcntl(_socket_to_accept, F_SETFL, O_NONBLOCK);
    if (return_fd < 0){
        perror("webserv error2 (fcntl)");
        exit(1);
    }
    std::cerr << "fd of accepted socket after is: " << _socket_to_accept << std::endl;
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
    _buffer_complete.append(std::string(_buffer));
    std::cout << "check what: " << position << std::endl;

    size_t find_host = _buffer_complete.find("Host:");
    size_t find_next_cr = 0;
    if (find_host != std::string::npos)
    {
        find_next_cr = _buffer_complete.substr(find_host , _buffer_complete.size()).find("\r\n");
        _request_hostname =  _buffer_complete.substr(find_host + std::string("Host: ").size() , find_next_cr - std::string("Host: ").size());
    }
    int which_serv = -1;
    for (int i = 0; i < _parse->num_serv; ++i) {
        if (_parse->serv[i]->server_name == _request_hostname) {
            which_serv = i;
            break ;
        }
        else if ((_parse->serv[i]->host + ":" + std::to_string(_parse->serv[i]->ind_port)) == _request_hostname) {
            which_serv = i;
            break ;
        }
    }
    if (which_serv == -1) {
        which_serv = 0;
    }
    _num_serv = which_serv;
    _buffer_complete.append("\0");
	return (_first_read_size);
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

int     Server::get_num_serv() const {
    return _num_serv;
}

int Server::getSocket_to_accept() const
{
    return this->_socket_to_accept;
}
