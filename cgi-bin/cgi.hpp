/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:33 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/17 14:32:51 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../parse/parsing.hpp"
# include <iostream>
# include <map>
# include <stdlib.h>
# include <sstream>
# include <fstream>
# include <cstdio>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>

class Request;

class CGI
{
private:
   // std::string _methode;
   // std::string _root_directory;
   // std::string _url_directory;
   // // std::string _cgi_name;
   // std::string _server_name;
   // std::string _http_host;
   char *_pwd;
   std::vector<std::string> hold;
   std::vector<std::string> _envcgi;
   char** _env;
   char * _cgi_script;
   
   // std::map<std::string,std::string> _env_cgi;
   // int         _server_port;
   
public:
    CGI();
    ~CGI();
 void fill_cgi(char *buffer, t_server *serv);
 void handle_cgi_request(Request& req);
};

# include "../Request/Request.hpp"

#endif

