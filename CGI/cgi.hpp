/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:33 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/27 11:35:29 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "../parse/parsing.hpp"
#include <iostream>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../utils/utils.hpp"

class Request;

class CGI
{
private:
      char *_pwd;
      int _location_index;
      int _server_index;
      int _fd_val;
      int _checker;
      std::vector<std::string> hold;
      std::vector<std::string> _envcgi;
      char **_env;
      std::string _cgi_script;
      std::string _script_name;
      std::string resp_buffer;
      std::string _ext;
      std::string executable;
      bool flag;
      std::string hold_ContentType;

public:
      CGI(int loc_index, int serv_index);
      ~CGI();
      void fill_cgi(std::map<std::string, std::string> header, std::string buffer, t_server *serv);
      int handle_cgi_request(Request &req, char const *buffer, t_server *serv);
      std::string const &getHoldbuffer() const;
      std::string const &getRespBuffer() const;
      void check_cgi(std::vector<std::string> str);
      std::string const &getContentType() const;
      void fill_env(std::string buffer);
      int  get_fd_val() const;
      int  get_checker() const;
      int  get_server_index() const;
};

#include "../Request/Request.hpp"

#endif