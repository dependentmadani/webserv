/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:33 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/11 10:55:21 by sriyani          ###   ########.fr       */
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
   char *_pwd;
   int  _location_index;
   int  _server_index;
   std::vector<std::string> hold;
   std::vector<std::string> _envcgi;
   char** _env;
   std::string _cgi_script;
   std::string  resp_buffer;
   
public:
      CGI(int loc_index, int serv_index);
      ~CGI();
      void fill_cgi(char *buffer, t_server *serv);
      void handle_cgi_request(Request& req);
      std::string const& getHoldbuffer() const;
      std::string const& getRespBuffer() const;
};

# include "../Request/Request.hpp"

#endif

