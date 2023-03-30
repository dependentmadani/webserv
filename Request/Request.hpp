/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:50 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/27 12:16:51 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../Server/Server.hpp"
#include <iostream>

class Request {

    private:
        Server      _server;
        int         _error_status;
        std::string _method;
        std::string _path;
        std::string _protocol;


    public:
        void    FirstLinerRequest();
        void    HeaderRequest();
        void    BodyRequest();

        std::string getMethod();
        std::string getPath();
        std::string getProtocol();
};

#endif
