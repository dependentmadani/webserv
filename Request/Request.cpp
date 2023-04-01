/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/27 12:16:45 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _method(), _path(), _protocol() ,_header(), http_code()
{
    _error_status = 0;
}

Request::~Request()
{ }

int     Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');

    this->FirstLinerRequest(splited_request[0]);
    this->HeaderRequest(request_message);
    return 0;
}

int    Request::FirstLinerRequest(char *request_message)
{
    _first_liner_header = std::string(request_message);
    // std::cout << "_first_liner value: " << _first_liner_header << std::endl;
    char **split_first_liner = ft_split(request_message, ' ');
    _method = std::string(split_first_liner[0]);
    if (!split_first_liner[1])
        return -1;
    _path = std::string(split_first_liner[1]);
    if (!split_first_liner[2])
        return -1;
    _protocol = std::string(split_first_liner[2]);
    // std::cout << "_method value: " << _method << std::endl;
    // std::cout << "_path value: " << _path << std::endl;
    // std::cout << "_protocol value: " << _protocol << std::endl;
    return 0;
}

int    Request::HeaderRequest(char *request_message)
{
    // char **splited_header = ft_split(request_message, '\r');
    char *splited_header = strtok(request_message, "\r\n");

    splited_header = strtok(NULL, "\r\n");
    while (splited_header != NULL)
    {
        char **split_each_line = ft_split(splited_header, ':');
        _header[std::string(split_each_line[0])] = std::string(split_each_line[1]);
        splited_header = strtok(NULL, "\r\n");
    }
    std::cout << "all good for now :) !" << std::endl;
    return 0;
}

void    Request::ft_http_code()
{
    // 2xx success
    http_code[200] = std::string("OK");
    http_code[201] = std::string("Created");
    http_code[202] = std::string("Accepted");
    http_code[204] = std::string("No Content");
    // 3xx redirection
    http_code[300] = std::string("Multiple Choices");
    http_code[301] = std::string("Moved Permanently");
    http_code[302] = std::string("Found");
    http_code[303] = std::string("See Other");
    http_code[304] = std::string("Not Modified");
    // 4xx client errors
    http_code[400] = std::string("Bad Request");
    http_code[401] = std::string("Unauthorized");
    http_code[403] = std::string("Forbidden");
    http_code[404] = std::string("Not Found");
    http_code[405] = std::string("Method Not Allowed");
    http_code[406] = std::string("Not Acceptable");
    http_code[409] = std::string("Conflict");
    http_code[410] = std::string("Gone");
    http_code[412] = std::string("Precondition Failed");
    http_code[413] = std::string("Payload Too Large");
    http_code[414] = std::string("URI Too Long");
    http_code[415] = std::string("Unsupported Media Type");
    // 5xx server errors
    http_code[500] = std::string("Internal Server Error");
    http_code[501] = std::string("Not Implemented"); 
    http_code[503] = std::string("Service Unavailable");
}

