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

Request::Request() : _method(), _path(), _protocol() ,_header()
{
    _error_status = 0;
}

Request::~Request()
{ }

int     Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');

    this->FirstLinerRequest(splited_request[0]);
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
}

int    Request::HeaderRequest()
{
    
}

