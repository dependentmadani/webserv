/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:33 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/10 18:44:47 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#include <iostream>
#include <map>
#include "../parse/parsing.hpp"
#include "../Request/Request.hpp"
class CGI
{
private:
   char *_pwd;
   std::vector<std::string> hold;
   std::vector<std::string> _envcgi;
   char** _env;
   std::string _cgi_script;
   std::string  resp_buffer;
   
public:
      CGI();
      ~CGI();
      void fill_cgi(char *buffer, t_server *serv);
      void handle_cgi_request(Request& req);
      std::string const& getHoldbuffer()const;
};

#endif

