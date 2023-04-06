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
# include "../parse/parsing.hpp"
# include <iostream>
# include <map>
# include <sys/stat.h>

class Request {

    private:
        Server                              _server;
        s_parsing                          *_parse;
        int                                 _http_status;
        std::vector<std::string>            _file_name_path;
        std::string                         _first_liner_header;
        std::string                         _method;
        std::string                         _path;
        std::string                         _protocol;
        std::string                         _body;
        std::map<std::string, std::string>  _header;


    public:
        Request();
        ~Request();

        int     ParseRequest(char* request_message);
        int     UseMethod();
        int     FirstLinerRequest(char *request_message);
        int     HeaderRequest(char *request_message);
        void    ft_http_code();
        void    ft_mime_type();
        int     ft_http_status(int value);
        int     check_method_protocol();
        int     is_request_well_formed(char *request_message);
        int     is_body_size_good(char *request_message);
        int     get_matched_location_for_request_uri();
        int     is_location_have_redirection();
        int     is_method_allowed_in_location();
        int     GET_method();
        int     get_request_resource();
        int     POST_method();
        int     DELETE_method();

        //function to check in the header
        int     is_available(std::string, std::string);
        //function to check url for a character not allowed
        int     url_characters_checker();
        void    print_parse_vector();
        std::string remove_space(std::string tmp);


        std::map<int, std::string>          http_code;
        std::map<std::string, std::string>  mime_type;
        std::vector<std::string>            allowed_methods;

        std::string getMethod() const;
        std::string getPath() const;
        std::string getProtocol() const;
        std::string getHeader() const;
        int         getHttpStatus() const;
        void        setParse(s_parsing *);
};

#endif
