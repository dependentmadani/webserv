/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:46:08 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/04/12 16:46:08 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi()
{}

cgi::~cgi()
{}

cgi::cgi(std::string code, Request request, std::string body)
{
    (void)code, (void)request, (void)body;
}

std::string get_data()
{
    return NULL;
}