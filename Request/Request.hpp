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

# include "../Server/Server.hpp"
# include "../utils/utils.hpp"
# include <iostream>
# include <map>

class Request {

    private:
        Server                              _server;
        int                                 _http_status;
        std::string                         _first_liner_header;
        std::string                         _method;
        std::string                         _path;
        std::string                         _protocol;
        std::map<std::string, std::string>  _header;


    public:
        Request();
        ~Request();

        int     ParseRequest(char* request_message);
        int     FirstLinerRequest(char *request_message);
        int     HeaderRequest(char *request_message);
        void    ft_http_code();
        void    ft_mime_type();
        int     ft_http_status(int value);
        int     check_method();
        int     is_request_well_formed();

        //function to check in the header
        int     is_available(std::string, std::string);
        //function to check url for a character not allowed
        int     url_characters_checker();

        std::map<int, std::string>          http_code;
        std::map<std::string, std::string>  mime_type;

        std::string getMethod() const;
        std::string getPath() const;
        std::string getProtocol() const;
        std::string getHeader() const;
        int         getHttpStatus() const;
};

#endif
