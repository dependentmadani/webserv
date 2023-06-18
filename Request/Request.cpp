/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/28 11:24:06 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../CGI/cgi.hpp"

Request::Request() : _list_files_directories(), _directory_to_upload_in(), _buffer(), _requested_file_path(), _directory_path(), _method(), _path(), _arguments(), _protocol(), _body(), _header(), http_code(), allowed_methods()
{
    char buffer[100];
    getcwd(buffer, 100);
    _current_directory = std::string(buffer) + "/public";
    _server_index = 0;
    _location_index = 0;
    _http_status = 200;
    _content_length = 0;
    _read_fd = 0;
    _final_file_size = 0;
    _chunked_content_value = 0;
    read_again = false;
}

Request::~Request()
{
}

void Request::clear_request_class()
{
    _http_status = 0;
    _file_directory_check = 0;
    _location_index = -1;
    _content_length = 0;
    _read_fd = 0;
    _chunked_content_value = 0;
    _final_file_size = 0;
    _file_name_path.clear();
    _list_files_directories.clear();
    _arguments.clear();
    _header.clear();
    _response_final.clear();
    Response.clear();
    Response = "";
    _available_file_path.clear();
    _available_file_path = "";
    _directory_path.clear();
    _directory_path = "";
    _first_liner_header.clear();
    _first_liner_header = "";
    _method.clear();
    _method = "";
    _path.clear();
    _path = "";
    _protocol.clear();
    _protocol = "";
    _directory_to_upload_in.clear();
    _directory_to_upload_in = "";
    _body.clear();
    _body = "";
    _response_body_as_string.clear();
    _response_body_as_string = "";
}

int Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');
    _buffer = std::string(request_message);

    this->clear_request_class();
    if (this->FirstLinerRequest(splited_request[0]) == 1)
    {
        std::cerr << "first_liner_request" << std::endl;
        free_doublep(splited_request);
        return 1;
    }
    free_doublep(splited_request);
    if (this->HeaderRequest(request_message)){
        std::cerr << "header request" << std::endl;
        return 1;
    }
    if (this->get_location_index() == -1)
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    if (check_method_protocol())
        return ft_http_status(getHttpStatus());
    if (this->is_request_well_formed(request_message))
        return ft_http_status(this->getHttpStatus());
    if (this->get_matched_location_for_request_uri())
        return ft_http_status(this->getHttpStatus());
    if (this->is_location_have_redirection())
        return ft_http_status(this->getHttpStatus());
    if (this->is_method_allowed_in_location())
    {
        _http_status = 405;
        return ft_http_status(getHttpStatus());
    }
    this->reform_requestPath_locationPath();
    if (_header.find("Content-Length") != _header.end())
        _content_actual_size = string_to_decimal(_header["Content-Length"]) - _server.getFirstReadSize();
    return 0;
}

int Request::FirstLinerRequest(char *request_message)
{
    if (!request_message)
        return 1;
    _first_liner_header = std::string(request_message);
    char **split_first_liner = ft_split(request_message, ' ');

    _method = std::string(split_first_liner[0]);
    if (!split_first_liner[1])
    {
        free_doublep(split_first_liner);
        return -1;
    }
    this->check_for_arguments_in_path(std::string(split_first_liner[1]));
    if (!split_first_liner[2])
    {
        free_doublep(split_first_liner);
        return -1;
    }
    _protocol = std::string(split_first_liner[2]);
    free_doublep(split_first_liner);
    return 0;
}

int Request::HeaderRequest(char *request_message)
{
    char **splited_header = ft_split(request_message, '\n');

    if (!strlen(request_message))
        return 1;
    int i = 1;
    while (splited_header[i] != NULL)
    {
        if (std::string(splited_header[i]).find(":") != std::string::npos)
        {
            char **split_each_line = ft_split(splited_header[i], ':');
            _header[std::string(split_each_line[0])] = std::string(split_each_line[1]).erase(0, 1);
            free_doublep(split_each_line);
            i++;
        }
        else
            break;
    }
    std::cerr << "the transfer encoding: " << _header["Content-Type"] << "|" << std::endl;
    std::string tmp(request_message);
    size_t position_empty_line = tmp.find("\r\n\r\n");
    if (position_empty_line != std::string::npos && position_empty_line != tmp.size())
    {
        _body = tmp.substr(position_empty_line + 3, tmp.size());
    }
    free_doublep(splited_header);
    return 0;
}

int Request::get_location_index()
{

    for (int i = 0; i < _parse->serv[_server_index]->num_location; ++i)
    {
        int size_for_path = _parse->serv[_server_index]->loc[i]->url_location.size() > getPath().size() ? getPath().size() : _parse->serv[_server_index]->loc[i]->url_location.size();
        if (this->getPath().substr(0, size_for_path) == _parse->serv[_server_index]->loc[i]->url_location)
            _location_index = i;
    }
    if (_location_index == -1)
    {
        _location_index = 0;
        return -1;
    }
    return 1;
}

/*
All the methods:
       Method         = "OPTIONS"                ; Section 9.2
                      | "GET"                    ; Section 9.3
                      | "HEAD"                   ; Section 9.4
                      | "POST"                   ; Section 9.5
                      | "PUT"                    ; Section 9.6
                      | "DELETE"                 ; Section 9.7
                      | "TRACE"                  ; Section 9.8
                      | "CONNECT"                ; Section 9.9
*/
int Request::check_method_protocol()
{
    if (_method == "GET" || _method == "POST" || _method == "DELETE")
    {
        _http_status = 200;
        return 0;
    }
    if (_method == "OPTIONS" || _method == "HEAD" || _method == "PUT" || _method == "TRACE" || _method == "CONNECT")
    {
        _http_status = 501;
        return ft_http_status(this->getHttpStatus());
    }
    if (_protocol != "HTTP/1.1")
    {
        _http_status = 403; // to check the http status code after...!!
        return ft_http_status(this->getHttpStatus());
    }
    _http_status = 501;
    return ft_http_status(this->getHttpStatus());
}

int Request::is_request_well_formed(char *request_message)
{
    if (_method == "POST" && is_available(std::string("Transfer-Encoding"), std::string("chunked")))
    {
        _http_status = 501;
        return ft_http_status(getHttpStatus());
    }
    if (_method == "POST" && _header.count("Transfer-Encoding") && _header.count("Content-Length"))
    {
        std::cerr << "what the heeeeck" << std::endl;
        _http_status = 400;
        return ft_http_status(getHttpStatus());
    }
    if (url_characters_checker())
    {
        _http_status = 400;
        return ft_http_status(getHttpStatus());
    }
    if (getPath().size() > 2048)
    {
        _http_status = 414;
        return ft_http_status(getHttpStatus());
    }
    if (is_body_size_good(request_message))
    {
        _http_status = 413;
        return ft_http_status(getHttpStatus());
    }
    std::cout << "all good for now :) !" << std::endl;
    return 0;
}

int Request::get_matched_location_for_request_uri()
{
    // check if path is valid, for security reason
    int path_counter = 0;
    size_t pos = 0;
    std::string url = getPath();

    url.erase(0, 1);
    std::string tmp;
    while ((pos = url.find("/")) != std::string::npos)
    {
        tmp = url.substr(0, pos);
        path_counter = ((tmp == "..") ? --path_counter : ++path_counter);
        if (path_counter < 0)
        {
            _http_status = 400;
            return ft_http_status(getHttpStatus());
        }
        url.erase(0, pos + 1);
    }
    // need to check if its available
    bool check_availability = false;

    int size_for_path = _parse->serv[_server_index]->loc[_location_index]->url_location.size() > getPath().size() ? getPath().size() : _parse->serv[_server_index]->loc[_location_index]->url_location.size();
    if (this->getPath().substr(0, size_for_path) == _parse->serv[_server_index]->loc[_location_index]->url_location)
        check_availability = true;

    if (!check_availability)
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int Request::is_location_have_redirection() //TODO: check the utility of this function
{
    for (size_t i = 0; i < _parse->serv[_server_index]->loc[_location_index]->location.size(); ++i)
    {
        if (_parse->serv[_server_index]->loc[_location_index]->location[i].find("return ") != std::string::npos)
        {
            size_t position_of_return = _parse->serv[_server_index]->loc[_location_index]->location[i].find("return") + 10;
            _http_status = 301;
            _path = _parse->serv[_server_index]->loc[_location_index]->location[i].substr(position_of_return, _parse->serv[_server_index]->loc[_location_index]->location[i].size());
            _path = remove_space(_path);
            return ft_http_status(getHttpStatus());
        }
    }
    return 0;
}

int Request::is_method_allowed_in_location()
{
    for (size_t i = 0; i < _parse->serv[_server_index]->loc[_location_index]->methods.size(); ++i)
    {
        if (this->getMethod() == _parse->serv[_server_index]->loc[_location_index]->methods[i])
            return 0;
    }
    return 1;
}


void Request::reform_requestPath_locationPath()
{
    std::string get_root;
    struct stat stat_buff;
    int i = 0;

    i = _location_index;
    if (this->getPath()[_parse->serv[_server_index]->loc[i]->url_location.size()] == '/' && this->getPath().size() != 1)
        get_root = this->getPath().substr(_parse->serv[_server_index]->loc[i]->url_location.size() + 1, this->getPath().size());
    else if (this->getPath().size() == 1)
        get_root = this->getPath();
    else
        get_root = this->getPath().substr(_parse->serv[_server_index]->loc[i]->url_location.size(), this->getPath().size());
    while (get_root[get_root.size() - 1] == '/' && this->getPath().size() != 1)
    {
        get_root.resize(get_root.size() - 1);
    }
    std::string complete_path;
    if (get_root[get_root.size() - 1] == '/' && _parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location;
    else if (_parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + get_root;
    else if (_parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/' && get_root[0] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + get_root.erase(0, 1);
    else
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + "/" + get_root;

    while (complete_path[complete_path.size() - 1] == '/')
    {
        complete_path.resize(complete_path.size() - 1);
    }
    stat(complete_path.c_str(), &stat_buff);
    std::cerr << "the compleeete path: " << complete_path << std::endl;
    if (S_ISDIR(stat_buff.st_mode))
    {
        std::cout << "complete path: " << complete_path << std::endl;
        _directory_path = complete_path;
        _file_directory_check = DIRECTORY;
        if (!_parse->serv[_server_index]->loc[i]->index.empty())
        {
            for (size_t index_indexes = 0; index_indexes < _parse->serv[_server_index]->loc[i]->index.size(); ++index_indexes)
            {
                _file_name_path.push_back(complete_path + "/" + _parse->serv[_server_index]->loc[i]->index[index_indexes]);
            }
        }
    }
    else if (S_ISREG(stat_buff.st_mode))
    {
        _file_directory_check = FILE;
        _content_length = stat_buff.st_size;
        _file_name_path.push_back(complete_path);
    }
    else
    {
        _file_directory_check = ERROR;
    }
}

// Use the method requested from the client

int Request::UseMethod()
{
    std::cerr << "the method used: " << _method << std::endl;
    if (_method == "GET")
        return this->GET_method();
    else if (_method == "POST") 
        return this->POST_method();
    else if (_method == "DELETE")
        return this->DELETE_method();
    return 0;
}

int Request::if_location_has_cgi()
{
    if (_parse->serv[_server_index]->loc[_location_index]->cgi_pass.empty())
    {
        return 0;
    }
    // call the constructor of cgi, than get the data from cgi. All of that as an else condition
    return 1;
}

int Request::Is_directory()
{
    if ( is_uri_has_backslash_in_end() )
    {
        if ( !is_dir_has_index_files() )
        {
            if (!get_auto_index())
            {
                _http_status = 403;
                return ft_http_status(getHttpStatus());
            }
            else
            {
                // build an autoindex page in response.
                this->build_autoindex_page();
                _http_status = 200;
                return ft_http_status(getHttpStatus());
            }
        }
        else
        {
            // if this directory has an index file, it should check for cgi in location
            if (this->if_location_has_cgi())
                return this->request_run_cgi();
        }
    }
    else
    {
        //resend a request with uri joined with backslash in end
        _http_status = 301;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int Request::is_uri_has_backslash_in_end()
{
    if (_path[_path.size() - 1] == '/')
        return 1;
    return 0;
}

int Request::is_dir_has_index_files()
{
    if (!_parse->serv[_server_index]->loc[_location_index]->index.empty())
        return 1;
    return 0;
}

bool Request::get_auto_index()
{
    return _parse->serv[_server_index]->loc[_location_index]->auto_index;
}

int Request::Is_file()
{
    if (if_location_has_cgi())
    {
        std::cerr << "kan hnaa a hmida rass lmida" << std::endl;
        return request_run_cgi();
        // _response_body_as_string = ;
    }
    if (access(_available_file_path.c_str(), R_OK) != 0)
    {
        _http_status = 403;
        return ft_http_status(getHttpStatus());
    }
    _response_body_as_string = read_file(_available_file_path);
    _http_status = 200;
    return ft_http_status(getHttpStatus());
}

int Request::check_for_arguments_in_path(std::string path)
{
    size_t position = path.find("?");
    if (position != std::string::npos)
    {
        char *catcher = strtok((char *)path.c_str(), "?");
        _path = std::string(catcher);
        if ((position + 1) == path.size())
            return 1;
        catcher = strtok(NULL, "?");
        std::string args = std::string(catcher);
        char **splited_args = ft_split(args.c_str(), '&');
        int i = 0;
        while (splited_args[i])
        {
            std::string tmp = splited_args[i];
            size_t pos = tmp.find("=");
            _arguments[tmp.substr(0, pos)] = tmp.substr(pos + 1, tmp.size());
            i++;
        }
        free_doublep(splited_args);
    }
    else
    {
        _path = path;
    }
    return 0;
}

std::string Request::read_file(std::string file)
{
    int fd_file = open(file.c_str(), O_RDONLY);
    int size_of_read = 1;
    char buffer_tmp[BUFFER_SIZE];
    std::string final_output;

    _final_file_size = 0;
    if (fd_file != -1)
    {
        while (size_of_read > 0)
        {
            memset(buffer_tmp, 0, BUFFER_SIZE);
            size_of_read = read(fd_file, buffer_tmp, BUFFER_SIZE);
            for (int i = 0; i < size_of_read; ++i)
            {
                final_output += buffer_tmp[i];
            }
            _final_file_size += size_of_read;
            std::cerr << "the size that it reads: " << size_of_read << std::endl;
        }
    }
    if (fd_file > 0)
        close(fd_file);
    // if ((int)final_output.size() == _final_file_size)
    //     _final_file_size = 0;
    // std::cerr << "should check the final size: " << _final_file_size << " and " << final_output.size() << std::endl;
    return final_output;
}

void Request::ft_mime_type()
{
    // text
    mime_type["php"] = "text/html";
    mime_type["html"] = "text/html";
    mime_type["htm"] = "text/html";
    mime_type["shtml"] = "text/html";
    mime_type["css"] = "text/css";
    mime_type["xml"] = "text/xml";
    // image
    mime_type["gif"] = "image/gif";
    mime_type["jpeg"] = "image/jpeg";
    mime_type["jpg"] = "image/jpeg";
    // application
    mime_type["js"] = "application/javascript";
    mime_type["atom"] = "application/atom+xml";
    mime_type["rss"] = "application/rss+xml";
    // text
    mime_type["mml"] = "text/mathml";
    mime_type["txt"] = "text/plain";
    mime_type["jad"] = "text/vnd.sun.j2me.app-descriptor";
    mime_type["wml"] = "text/vnd.wap.wml";
    mime_type["htc"] = "text/x-component";
    // image
    mime_type["avif"] = "image/avif";
    mime_type["png"] = "image/png";
    mime_type["svg"] = "image/svg+xml";
    mime_type["svgz"] = "image/svg+xml";
    mime_type["tif"] = "image/tiff";
    mime_type["tiff"] = "image/tiff";
    mime_type["wbmp"] = "image/vnd.wap.wbmp";
    mime_type["webp"] = "image/webp";
    mime_type["ico"] = "image/x-icon";
    mime_type["jng"] = "image/x-jng";
    mime_type["bmp"] = "image/x-ms-bmp";
    // font
    mime_type["woff"] = "font/woff";
    mime_type["woff2"] = "font/woff2";
    // application
    mime_type["jar"] = "application/java-archive";
    mime_type["war"] = "application/java-archive";
    mime_type["ear"] = "application/java-archive";
    mime_type["json"] = "application/json";
    mime_type["hqx"] = "application/mac-binhex40";
    mime_type["doc"] = "application/msword";
    mime_type["pdf"] = "application/pdf";
    mime_type["ps"] = "application/postscript";
    mime_type["eps"] = "application/postscript";
    mime_type["ai"] = "application/postscript";
    mime_type["rtf"] = "application/rtf";
    mime_type["m3u8"] = "application/vnd.apple.mpegurl";
    mime_type["kml"] = "application/vnd.google-earth.kml+xml";
    mime_type["kmz"] = "application/vnd.google-earth.kmz";
    mime_type["xls"] = "application/vnd.ms-excel";
    mime_type["eot"] = "application/vnd.ms-fontobject";
    mime_type["ppt"] = "application/vnd.ms-powerpoint";
    mime_type["odg"] = "application/vnd.oasis.opendocument.graphics";
    mime_type["odp"] = "application/vnd.oasis.opendocument.presentation";
    mime_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mime_type["odt"] = "application/vnd.oasis.opendocument.text";
    mime_type["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime_type["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_type["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime_type["wmlc"] = "application/vnd.wap.wmlc";
    mime_type["wasm"] = "application/wasm";
    mime_type["7z"] = "application/x-7z-compressed";
    mime_type["cco"] = "application/x-cocoa";
    mime_type["jardiff"] = "application/x-java-archive-diff";
    mime_type["jnlp"] = "application/x-java-jnlp-file";
    mime_type["run"] = "application/x-makeself";
    mime_type["pl"] = "application/x-perl";
    mime_type["pm"] = "application/x-perl";
    mime_type["prc"] = "application/x-pilot";
    mime_type["pdb"] = "application/x-pilot";
    mime_type["rar"] = "application/x-rar-compressed";
    mime_type["rpm"] = "application/x-redhat-package-manager";
    mime_type["sea"] = "application/x-sea";
    mime_type["swf"] = "application/x-shockwave-flash";
    mime_type["sit"] = "application/x-stuffit";
    mime_type["tcl"] = "application/x-tcl";
    mime_type["tk"] = "application/x-tcl";
    mime_type["der"] = "application/x-x509-ca-cert";
    mime_type["pem"] = "application/x-x509-ca-cert";
    mime_type["crt"] = "application/x-x509-ca-cert";
    mime_type["xpi"] = "application/x-xpinstall";
    mime_type["xhtml"] = "application/xhtml+xml";
    mime_type["xspf"] = "application/xspf+xml";
    mime_type["zip"] = "application/zip";
    mime_type["bin"] = "application/octet-stream";
    mime_type["exe"] = "application/octet-stream";
    mime_type["dll"] = "application/octet-stream";
    mime_type["deb"] = "application/octet-stream";
    mime_type["dmg"] = "application/octet-stream";
    mime_type["iso"] = "application/octet-stream";
    mime_type["img"] = "application/octet-stream";
    mime_type["msi"] = "application/octet-stream";
    mime_type["msp"] = "application/octet-stream";
    mime_type["msm"] = "application/octet-stream";
    // audio
    mime_type["mid"] = "audio/midi";
    mime_type["midi"] = "audio/midi";
    mime_type["kar"] = "audio/midi";
    mime_type["mp3"] = "audio/mpeg";
    mime_type["ogg"] = "audio/ogg";
    mime_type["m4a"] = "audio/x-m4a";
    mime_type["ra"] = "audio/x-realaudio";
    // video
    mime_type["3gpp"] = "video/3gpp";
    mime_type["3gp"] = "video/3gpp";
    mime_type["ts"] = "video/mp2t";
    mime_type["mp4"] = "video/mp4";
    mime_type["mpeg"] = "video/mpeg";
    mime_type["mpg"] = "video/mpeg";
    mime_type["mov"] = "video/quicktime";
    mime_type["webm"] = "video/webm";
    mime_type["flv"] = "video/x-flv";
    mime_type["m4v"] = "video/x-m4v";
    mime_type["mng"] = "video/x-mng";
    mime_type["asx"] = "video/x-ms-asf";
    mime_type["asf"] = "video/x-ms-asf";
    mime_type["wmv"] = "video/x-ms-wmv";
    mime_type["avi"] = "video/x-msvideo";
}

void Request::ft_http_code()
{
    // 2xx success
    http_code[200] = std::string("OK");
    http_code[201] = std::string("Created");
    http_code[202] = std::string("Accepted");
    http_code[203] = std::string("Non-Authoritative Information");
    http_code[204] = std::string("No Content");
    // 3xx redirection
    http_code[300] = std::string("Multiple Choices");
    http_code[301] = std::string("Moved Permanently");
    http_code[302] = std::string("Found");
    http_code[303] = std::string("See Other");
    http_code[304] = std::string("Not Modified");
    // 4xx client errors
    http_code[400] = std::string("Bad Request");
    http_code[401] = std::string("Unauthorized");
    http_code[403] = std::string("Forbidden");
    http_code[404] = std::string("Not Found");
    http_code[405] = std::string("Method Not Allowed");
    http_code[406] = std::string("Not Acceptable");
    http_code[409] = std::string("Conflict");
    http_code[410] = std::string("Gone");
    http_code[412] = std::string("Precondition Failed");
    http_code[413] = std::string("Payload Too Large");
    http_code[414] = std::string("URI Too Long");
    http_code[415] = std::string("Unsupported Media Type");
    // 5xx server errors
    http_code[500] = std::string("Internal Server Error");
    http_code[501] = std::string("Not Implemented");
    http_code[503] = std::string("Service Unavailable");
    http_code[504] = std::string("Gateway Timeout");
}

int Request::is_available(std::string key, std::string value)
{
    int val = _header.count(key);

    if (val && _header[key].find("\r") != std::string::npos)
        _header[key].pop_back();

    if (val && _header[key] != value)
    {
        std::cerr << "should be heeere i guess" << std::endl;
        return 1;
    }
    return 0;
}

int Request::url_characters_checker()
{
    size_t return_value = _path.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%");
    if (return_value != std::string::npos)
        return 1;
    return 0;
}

int Request::is_body_size_good(char *request_message)
{
    (void)request_message;
    if (_body.size() > (size_t)_parse->serv[_server_index]->max_client)
        return 1;
    return 0;
}

std::string Request::getPath() const
{
    return _path;
}

std::map<std::string, std::string> Request::getHeader() const
{
    return _header;
}

int Request::getHttpStatus() const
{
    return _http_status;
}

std::string Request::getProtocol() const
{
    return _protocol;
}

std::string Request::getMethod() const
{
    return _method;
}

std::string Request::getResponse()
{
    return _response_body_as_string;
}

std::string Request::get_server_buffer() const
{
    return _buffer;
}

std::string Request::getAvailableFilePath() const
{
    return _available_file_path;
}

int Request::getFile_size() const
{
    return _final_file_size;
}

void Request::setParse(s_parsing *parsed)
{
    this->_parse = parsed;
}

void Request::setServer(Server serv)
{
    this->_server = serv;
}

void Request::setServer_index(int index)
{
    this->_server_index = index;
}

std::string Request::remove_space(std::string tmp)
{
    std::string finale = "";

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        if (tmp[i] != ' ')
            finale += tmp[i];
    }
    return finale;
}

std::string const &Request::getBody() const
{
    return _body;
}

void Request::set_read_fd(int fd)
{
    _read_fd = fd;
}

unsigned long Request::hex_to_dec(std::string hex)
{
    unsigned long output = 0;
    for (size_t i = 0; i < hex.length(); ++i)
    {
        if (hex[i] >= 48 && hex[i] <= 57)
        {
            output += (hex[i] - 48) * std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70)
        {
            output += (hex[i] - 55) * std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102)
        {
            output += (hex[i] - 87) * std::pow(16, hex.length() - i - 1);
        }
    }
    return output;
}

int Request::string_to_decimal(std::string str)
{
    std::stringstream converted;
    int number;

    converted << str;
    converted >> number;

    return number;
}
