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

Server::Server() : _first_read_size(), _host_addr(), _bind_address(), _hints(), _socket_client(), _buffer_complete(), _request_hostname()
{
    _num_serv = 0;
    _socket_fd = 0;
    _socket_to_accept = 0;
    _port = 8080;
    _connexion_status = false;
}

Server::Server(int port) : _first_read_size(), _host_addr(), _bind_address(), _hints(), _socket_client(), _buffer_complete(), _request_hostname()
{
    _num_serv = 0;
    _socket_fd = 0;
    _socket_to_accept = 0;
    _port = port;
    _connexion_status = false;
}

Server::~Server()
{
}

int Server::initiate_socket(int pos)
{
    std::ostringstream converted;
    memset(&_hints, 0, sizeof(_hints));

    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;

    converted << _port;
    getaddrinfo(_parse->serv[pos]->host.c_str(), converted.str().c_str(), &_hints, &_bind_address);
    converted.str("");
    converted.clear();

    _socket_fd = socket(_bind_address->ai_family, _bind_address->ai_socktype, _bind_address->ai_protocol);
    if (_socket_fd < 0)
    {
        perror("webserv error (socket) ");
        return -1;
    }
    int return_fd = fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
    if (return_fd < 0){
        perror("webserv error1 (fcntl)");
        return -1;
    }
    const int on = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0) {
        std::cerr << "Failed to set socket option" << std::endl;
    }
    int i = bind(_socket_fd, _bind_address->ai_addr, _bind_address->ai_addrlen);
    if (i < 0) {
        freeaddrinfo(_bind_address);
        perror("webserv error (bind) ");
        return (-1);
    }
    freeaddrinfo(_bind_address);
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

struct  Client  *Server::get_client(struct Client **client, int sock) {
    struct Client *ci = *client;

    while (ci) {
        if (ci->socket == sock)
            break;
        ci = ci->next;
    }
    if (ci)
        return ci;
    
    struct Client *n = new Client();
    n->request = new Request();
    n->request->ft_http_code();
    n->request->ft_mime_type();
    n->request->setParse(_parse);
    n->address_length = sizeof(n->address);
    n->next = *client;
    *client = n;
    return n;
}

void    Server::drop_client(struct Client **cl, struct Client *client) {
    struct Client **p = cl;

    while (*p) {
        if (*p == client) {
            *p = client->next;
            delete client->request;
            delete client;
            std::cout << "\033[1;31mThe client is dropped\033[1;0m" << std::endl;
            return ;
        }
        p = &(*p)->next;
    }
}



void Server::accept_connections(int position)
{
    int addr_length = sizeof(_host_addr);

    _socket_to_accept = accept(position, (struct sockaddr *)&_host_addr, (socklen_t *)&addr_length);
    if (_socket_to_accept < 0)
    {
        perror("webserv error (accept)");
        return ;
    }
    int return_fd = fcntl(_socket_to_accept, F_SETFL, O_NONBLOCK);
    if (return_fd < 0){
        perror("webserv error1 (fcntl)");
        return ;
    }
}

int Server::recv_data(int position)
{
    std::ofstream file;
    std::ostringstream converted;
    file.open("temp_file");
    memset(_buffer, 0, BUFFER_SIZE);

    _first_read_size = recv(position, _buffer, BUFFER_SIZE, 0);
    if (_first_read_size < 0)
        return -1;
    for (int i = 0; i < _first_read_size; ++i) {
        file << _buffer[i];
    }
    _buffer_complete.clear();
    _buffer_complete.append(std::string(_buffer));

    size_t find_host = _buffer_complete.find("Host:");
    size_t find_next_cr = 0;
    if (find_host != std::string::npos)
    {
        find_next_cr = _buffer_complete.substr(find_host , _buffer_complete.size()).find("\r\n");
        _request_hostname =  _buffer_complete.substr(find_host + std::string("Host: ").size() , find_next_cr - std::string("Host: ").size());
    }
    int which_serv = -1;
    for (int i = 0; i < _parse->num_serv; ++i) {
        converted << _parse->serv[i]->ind_port;
        if ((_parse->serv[i]->host + ":" + converted.str()) == _request_hostname) {
            which_serv = i;
            converted.str("");
            converted.clear();
            break ;
        }
        converted.str("");
        converted.clear();
    }
    if (which_serv == -1) {
        for (int i = 0; i < _parse->num_serv; ++i) {
            if (_parse->serv[i]->server_name == _request_hostname) {
                which_serv = i;
                break ;
            }
        }
    }
    if (which_serv == -1) {
        return -2;
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
