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

# include "../server/Server.hpp"
# include "../utils/utils.hpp"
# include <iostream>
# include <map>

class Request {

    private:
        Server                              _server;
        int                                 _error_status;
        std::string                         _first_liner_header;
        std::string                         _method;
        std::string                         _path;
        std::string                         _protocol;
        std::map<std::string, std::string>  _header;


    public:
        Request();
        ~Request();

        int     ParseRequest(char* request_message);
        int    FirstLinerRequest(char *request_message);
        int    HeaderRequest();

        std::string getMethod() const;
        std::string getPath() const;
        std::string getProtocol() const;
        std::string getHeader() const;
};

#endif
