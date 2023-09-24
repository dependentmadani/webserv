/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:02:09 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/06/17 14:02:28 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::GET_method()
{
    if (this->get_request_resource() && !_parse->serv[_server_index]->loc[_location_index]->auto_index)
    {
        _http_status = 404;
        return this->ft_http_status(getHttpStatus());
    }
    if (this->get_resource_type() == DIRECTORY)
        return this->Is_directory();
    else if (this->get_resource_type() == FILE)
    {
        return this->Is_file();
    }
    else if (this->get_resource_type() == ERROR)
    {
        _http_status = 404;
        return this->ft_http_status(getHttpStatus());
    }
    return 1;
}