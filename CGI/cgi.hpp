/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:46:15 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/04/12 16:46:16 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <map>
# include <vector>
# include <iostream>
# include "../Request/Request.hpp"

class cgi {

    private:
        Request                             _request;
        std::string                         _cgi_path;
        std::string                         _cgi_body;
        std::string                         _code;
        std::map<std::string, std::string>  _map_env;
        char                              **_env;
        std::vector<std::string>            _data;

    public:

        cgi();
        cgi(std::string, Request, std::string);
        ~cgi();

        std::string get_data();

};

#endif
