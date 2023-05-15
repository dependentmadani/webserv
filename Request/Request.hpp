/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:50 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/10 14:52:28 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../Server/Server.hpp"
# include "../utils/utils.hpp"
# include "../parse/parsing.hpp"
# include "../cgi-bin/cgi.hpp"
# include <iostream>
# include <map>
# include <string>
# include <sstream>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/dir.h>
# include <unistd.h>


# define DIRECTORY  5
# define FILE       4
# define ERROR      -1

class Request {

    private:
        // std::map<std::string, std::string>  _response_body;
        Server                              _server;
        s_parsing                          *_parse;
        int                                 _http_status;
        int                                 _file_directory_check;
        int                                 _server_index;
        int                                 _location_index;
        int                                 _content_length;
        std::vector<std::string>            _file_name_path;
        std::string                         _available_file_path;
        std::string                         _directory_path;
        std::string                         _first_liner_header;
        std::string                         _method;
        std::string                         _path;
        std::map<std::string, std::string>  _arguments;
        std::string                         _protocol;
        std::string                         _body;
        std::map<std::string, std::string>  _header;
        std::map<std::string, std::string>  _response_final;
        std::string                         _response_body_as_string;

    public:
        Request();
        ~Request();

        void    clear_request_class();
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
        int     get_resource_type();
        int     if_location_has_cgi();
        int     POST_method();
        int     DELETE_method();

        int     Is_directory();
        void    build_autoindex_page();
        int     Is_directory_for_DELETE();
        int     Is_file_for_DELETE();
        int     is_uri_has_backslash_in_end();
        int     is_dir_has_index_files();
        bool    get_auto_index();
        int     delete_all_folder_content(std::string, int);
        int     has_write_access_on_folder();

        void    create_form_for_post();

        int     Is_file();

        //get which location
        void    get_location_index();
        //function to check in the header
        int     is_available(std::string, std::string);
        //function to check url for a character not allowed
        int     url_characters_checker();
        void    print_parse_vector();
        std::string remove_space(std::string tmp);
        //get the file_name_path value
        void    reform_requestPath_locationPath();
        std::string read_file(std::string);
        void    build_date();
        void    add_zero(int timer);
        int     check_for_arguments_in_path(std::string path);

        /*Response functions*/
        void    build_response();
        
          /* POST */
        int     upload_post_request();
        bool    location_support_upload();
        int     If_is_directory();
        bool    is_location_has_cgi();
        int     request_run_cgi();
        int     If_is_file();

        std::map<int, std::string>          http_code;
        std::map<std::string, std::string>  mime_type;
        std::vector<std::string>            allowed_methods;
        std::string                         Response;

        std::string getMethod() const;
        std::string getPath() const;
        std::string getProtocol() const;
        std::map<std::string, std::string> getHeader() const;
        std::string getResponse();
        int         getHttpStatus() const;
        void        setParse(s_parsing *);
        void        setServer_index(int index);
        std::string const& getBody() const;
};

#endif
