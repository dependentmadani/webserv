/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/21 13:50:24 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../CGI/cgi.hpp"

Request::Request() : _buffer(), _current_directory(), _requested_file_path(), _directory_path(), _method(), _path(), _arguments(), _protocol(), _body(), _header(), http_code(), allowed_methods()
{
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
    _arguments.clear();
    _header.clear();
    _response_final.clear();
    Response.clear();
    Response = "";
    _available_file_path.clear();
    _available_file_path = "";
    _current_directory.clear();
    _current_directory = "";
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
    _body.clear();
    _body = "";
    _response_body_as_string.clear();
    _response_body_as_string = "";
}

int Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');

    _buffer = std::string(request_message);
    if (!read_again)
        this->clear_request_class();
    char buffer[100];
    getcwd(buffer, 100);
    _current_directory = std::string(buffer) + "/public";
    if (this->FirstLinerRequest(splited_request[0]) == 1)
    {
        return 1;
    }
    if (this->HeaderRequest(request_message))
        return 1;
    if (this->get_location_index() == -1)
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    if (check_method_protocol())
        return ft_http_status(getHttpStatus());
    this->is_request_well_formed(request_message);
    this->get_matched_location_for_request_uri();
    this->is_location_have_redirection();
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

int Request::UseMethod()
{
    if (_method == "GET")
        return this->GET_method();
    else if (_method == "POST")
        return this->POST_method();
    else if (_method == "DELETE")
        return this->DELETE_method();
    return 0;
}

void Request::build_response()
{
    std::ostringstream converted;

    converted << this->getHttpStatus();
    // _response["http_version"] = "HTTP/1.1";
    // _response["return_code"] = converted.str();
    // _response["status"] = http_code[this->getHttpStatus()];
    Response = "HTTP/1.1 " + converted.str() + " " + http_code[this->getHttpStatus()] + "\r\n";
    // _response_final["Location"] = "http://" + _parse->serv[_server_index]->server_name +_path; // to make a variable to contruct the full location
    std::string file_type;

    int position_extension = _available_file_path.find_last_of(".");
    file_type = _available_file_path.substr(position_extension + 1, _available_file_path.size());
    if (!file_type.empty())
        _response_final["Content_Type"] = mime_type[file_type];
    if (this->getHttpStatus() == 301)
    {
        _response_final["Location"] = _arguments["Host"] + this->_path + "/";
    }
    converted.str("");
    converted.clear();
    converted << _content_length;
    _response_final["Connection"] = "closed";
    if (_content_length)
        _response_final["Content_Length"] = converted.str();

    // std::cout << "the string: " << _response["content_length"] << ", and: " << _response["return_code"] << std::endl;

    /*
    The response http should include:
    -The http version
    -The return code
    -The status
    -The date specifies the date and time the http response was generated
    -The server describes the webserver software used to generate the response
    -The content-length describes the length of the response
    -The content-type describes the media type of the resource returned.
    */
    //    Response.append("Date: Thu 20 Apr 2023 01:22:10 GMT\r\n");
    this->build_date();
    Response.append("Server: webserv/1.0\r\n");
    std::map<std::string, std::string>::iterator b = _response_final.begin();
    for (; b != _response_final.end(); ++b)
    {
        std::string add_line = b->first + ": " + b->second + "\r\n";
        Response.append(add_line);
    }
    Response.append("\r\n");
    Response.append(_response_body_as_string);
}

void Request::add_zero(int timer)
{
    if (timer >= 0 && timer < 10)
    {
        Response.append("0");
    }
}

void Request::build_date()
{
    time_t now = time(0);
    std::string days_of_week[8] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    std::string months[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    tm *time = localtime(&now);
    Response.append("Date: ").append(days_of_week[time->tm_wday]).append(", ");
    this->add_zero(time->tm_mday);
    Response.append(std::to_string(time->tm_mday)).append(" ");
    Response.append(months[time->tm_mon]).append(" ").append(std::to_string(time->tm_year + 1900)).append(" ");
    this->add_zero(time->tm_hour);
    Response.append(std::to_string(time->tm_hour)).append(":");
    this->add_zero(time->tm_min);
    Response.append(std::to_string(time->tm_min)).append(":");
    this->add_zero(time->tm_sec);
    Response.append(std::to_string(time->tm_sec)).append(" GMT").append("\r\n");
}

int Request::GET_method()
{
    if (this->get_request_resource() && !_parse->serv[_server_index]->loc[_location_index]->auto_index)
    {
        _http_status = 404;
        return this->ft_http_status(getHttpStatus());
    }
    // std::cerr << "Waaaaaaahya hamiiiiiiiiiid: " << this->get_resource_type() << std::endl;
    if (this->get_resource_type() == DIRECTORY)
        return this->Is_directory();
    else if (this->get_resource_type() == FILE)
    {
        return this->Is_file();
    }
    else if (this->get_resource_type() == ERROR) { 
        _http_status = 404;
        return this->ft_http_status(getHttpStatus());
    }
    return 1;
}

int Request::get_request_resource()
{
    struct stat stat_buff;

    for (std::vector<std::string>::iterator b = _file_name_path.begin(); b != _file_name_path.end(); ++b)
    {
        int stat_return = stat((*b).c_str(), &stat_buff);
        if (stat_return != -1)
        {
            _available_file_path = *b;
            _content_length = stat_buff.st_size;
            std::cout << "the file is available " << *(b) << std::endl;
            return 0;
        }
    }
    return 1;
}

int Request::if_location_has_cgi()
{
    if (_parse->serv[_server_index]->loc[_location_index]->cgi_pass.empty())
    {
        this->Is_file();
        _http_status = 200;
        ft_http_status(getHttpStatus());
        return 0;
    }
    // call the constructor of cgi, than get the data from cgi. All of that as an else condition
    this->request_run_cgi();
    _http_status = 200; // to check depends on cgi
    ft_http_status(getHttpStatus());
    return 1;
}

int Request::Is_directory()
{
    if (is_uri_has_backslash_in_end())
    {
        if (!is_dir_has_index_files())
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
            return this->if_location_has_cgi();
        }
    }
    else
    {
        // if ( !get_auto_index() )
        // {
        //     _http_status = 403;
        //     return ft_http_status(getHttpStatus());
        // }
        // redirect the request by adding "/" to the request path.
        // this->build_autoindex_page();
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
    int size_for_path = _parse->serv[_server_index]->loc[_location_index]->url_location.size() > getPath().size() ? getPath().size() : _parse->serv[_server_index]->loc[_location_index]->url_location.size();
    if (this->getPath().substr(0, size_for_path) == _parse->serv[_server_index]->loc[_location_index]->url_location)
    {
        if (!_parse->serv[_server_index]->loc[_location_index]->index.empty())
            return 1;
    }
    return 0;
}

bool Request::get_auto_index()
{

    // int size_for_path = _parse->serv[_server_index]->loc[_location_index]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[_server_index]->loc[_location_index]->url_location.size();
    // if (this->getPath().substr(0, size_for_path) == _parse->serv[_server_index]->loc[_location_index]->url_location)
    // {
    return _parse->serv[_server_index]->loc[_location_index]->auto_index;
    // }
    // return false;
}

int Request::Is_file()
{
    std::cerr << "shouuuuld be heeerreee i guess :((((((: ";
    // std::ifstream file;
    // // char line[BUFFER_SIZE];
    // // std::ifstream file

    // file.open(_available_file_path.c_str());
    // if (!_parse->serv[_server_index]->loc[_location_index]->cgi_pass.empty())
    //     return this->if_location_has_cgi();
    // if (file.is_open())
    // {
    //     // while (file)
    //     // {
    //         file.read(_buffer_char, BUFFER_SIZE);
    //         _response_body_as_string = std::string(_buffer_char);
    //         // _response_body_as_string.append(line);
    //     // }
    //     // close(file);
    // }
    // std::cerr << _response_body_as_string.size() << std::endl;
    _response_body_as_string.append(read_file(_available_file_path));
    return 0;
}

int Request::get_resource_type()
{
    return _file_directory_check;
}

int Request::DELETE_method()
{
    if (this->get_request_resource())
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    if (this->get_resource_type() == DIRECTORY)
        return this->Is_directory_for_DELETE();
    else if (this->get_resource_type() == FILE)
        return this->Is_file_for_DELETE();
    return 0;
}

int Request::Is_directory_for_DELETE()
{
    if (is_uri_has_backslash_in_end())
    {
        if (!if_location_has_cgi())
        {
            if (is_dir_has_index_files())
            {
                // run cgi on requested file with DELETE REQUEST METHOD
                // and check if this directory has an index file and cgi
                // then return code depend on cgi
                _http_status = this->request_run_cgi();
                return ft_http_status(getHttpStatus());
            }
            else
            {
                // build an autoindex page in response.
                _http_status = 403;
                return ft_http_status(getHttpStatus());
            }
        }
        else
        {
            // delete all folder content
            if (this->delete_all_folder_content(_directory_path, DIRECTORY))
            {
                // std::cout << "well, all is good" << std::endl;
                _http_status = 204;
                return ft_http_status(getHttpStatus());
            }
            else
            {
                // std::cout << "those things are not good" << std::endl;
                if (this->has_write_access_on_folder())
                {
                    _http_status = 500;
                    return ft_http_status(getHttpStatus());
                }
                else
                {
                    _http_status = 403;
                    return ft_http_status(getHttpStatus());
                }
            }
        }
    }
    else
    {
        // redirect the request by adding "/" to the request path.
        _http_status = 409;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int Request::Is_file_for_DELETE()
{
    if (this->if_location_has_cgi())
    {
        std::cout << "shouldnt be here aaaa hamid :)" << std::endl;
        // nothing to do here for the moment. waiting for cgi to be done.
        // return code depend on cgi
        _http_status = 200;
        return ft_http_status(getHttpStatus());
    }
    else
    {
        std::cout << "was here to delete" << std::endl;
        return this->delete_all_folder_content(_file_name_path[0], FILE);
    }
    return 0;
}

int Request::delete_all_folder_content(std::string folder_file, int type)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(folder_file.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {

            if ((!strcmp(ent->d_name, ".") || strcmp(ent->d_name, "..")) && (strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")))
            {
                std::string tmp;
                if (type == DIRECTORY)
                    tmp = folder_file + "/" + ent->d_name;
                else
                    tmp = folder_file;
                std::cout << "the file is: " << tmp << std::endl;
                std::cout << "the file would be: " << ent->d_name << std::endl;
                if (!std::remove(tmp.c_str()))
                {
                    std::cout << "well removed file" << std::endl;
                }
                else
                {
                    std::cout << "something wrong with file: " << ent->d_name << std::endl;
                    return 0;
                }
            }
        }
        closedir(dir);
    }
    else if (type == FILE)
    {
        std::cout << "I believe it reached here: " << folder_file << std::endl;
        if (!std::remove(folder_file.c_str()))
        {
            std::cout << "well removed the file" << std::endl;
        }
        else
        {
            std::cout << "something wrong in file" << std::endl;
            return 0;
        }
    }
    return 1;
}

int Request::has_write_access_on_folder()
{
    int check_access = 0;

    check_access = access(_directory_path.c_str(), W_OK);
    if (check_access != 0)
        return 0;
    return 1;
}

int Request::FirstLinerRequest(char *request_message)
{
    if (!request_message)
        return 1;
    _first_liner_header = std::string(request_message);

    // std::cout << "_first_liner value: " << _first_liner_header << std::endl;
    char **split_first_liner = ft_split(request_message, ' ');
    _method = std::string(split_first_liner[0]);
    if (!split_first_liner[1])
        return -1;
    this->check_for_arguments_in_path(std::string(split_first_liner[1]));
    if (!split_first_liner[2])
        return -1;
    _protocol = std::string(split_first_liner[2]);
    return 0;
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
    }
    else
    {
        _path = path;
    }
    return 0;
}

// int    Request::HeaderRequest(char *request_message)
// {
//     // char **splited_header = ft_split(request_message, '\r');
//     char *tmp = request_message;
//     char *tmp_request = request_message;
//     char *splited_header = strtok_r(tmp_request, "\r\n", &tmp);

//     splited_header = strtok_r(NULL, "\r\n", &tmp);
//     while (splited_header != NULL)
//     {
//         char **split_each_line = ft_split(splited_header, ':');
//         _header[std::string(split_each_line[0])] = std::string(split_each_line[1]);
//         splited_header = strtok_r(NULL, "\r\n", &tmp);
//         if (splited_header && !strcmp(splited_header,""))
//             std::cout << "kan hna" << std::endl;
//     }
//     // std::cout << "all good for now :) !" << std::endl;
//     return 0;
// }

int Request::HeaderRequest(char *request_message)
{
    char **splited_header = ft_split(request_message, '\n');
    // char *tmp = request_message;
    // char *tmp_request = request_message;
    // char *splited_header = strtok_r(tmp_request, "\r\n", &tmp);
    if (!strlen(request_message))
        return 1;
    int i = 1;
    while (splited_header[i] != NULL)
    {
        if (std::string(splited_header[i]).find(":") != std::string::npos)
        {
            char **split_each_line = ft_split(splited_header[i], ':');
            _header[std::string(split_each_line[0])] = std::string(split_each_line[1]).erase(0, 1);
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
    return 0;
}

int Request::is_request_well_formed(char *request_message)
{
    if (_method == "POST" && is_available(std::string("Transfer-Encoding"), std::string("chunked")))
    {
        _http_status = 501;
        return ft_http_status(getHttpStatus());
    }
    if (_method == "POST" && !is_available(std::string("Transfer-Encoding"), std::string("")) && !is_available(std::string("Content-Length"), std::string("")))
    {
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
        path_counter = ( (tmp == "..") ? --path_counter : ++path_counter );
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

int Request::is_location_have_redirection()
{
    for (size_t i = 0; i < _parse->serv[_server_index]->loc[_location_index]->location.size(); ++i)
    {
        if (_parse->serv[_server_index]->loc[_location_index]->location[i].find("return 301") != std::string::npos)
        {
            size_t position_of_return = _parse->serv[_server_index]->loc[_location_index]->location[i].find("return 301") + 10;
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
    // std::cerr <<
    // int size_for_path = _parse->serv[_server_index]->loc[_location_index]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[_server_index]->loc[_location_index]->url_location.size();
    // if (this->getPath().substr(0, size_for_path) == _parse->serv[_server_index]->loc[_location_index]->url_location)
    // {
    for (size_t i = 0; i < _parse->serv[_server_index]->loc[_location_index]->methods.size(); ++i)
    {
        if (this->getMethod() == _parse->serv[_server_index]->loc[_location_index]->methods[i])
            return 0;
    }
    // }
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
    // if (get_root == "/" && _parse->serv[_server_index]->loc[i]->root_location == "/")
    // complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location;
    // else if (_parse->serv[_server_index]->loc[i]->root_location == "/" || get_root == "/")
    //     complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + getPath();
    // else
    // complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + "/" + getPath();
    if (get_root[get_root.size() - 1] == '/' && _parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location;
    else if (_parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + get_root;
    else if (_parse->serv[_server_index]->loc[i]->root_location[_parse->serv[_server_index]->loc[i]->root_location.size() - 1] == '/' && get_root[0] == '/')
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + get_root.erase(0, 1);
    else
        complete_path = _current_directory + _parse->serv[_server_index]->loc[i]->root_location + "/" + get_root;

    // free(buffer);
    // std::cerr << "*******|" << complete_path << "|**********" << get_root << "|********" <<std::endl;
    while (complete_path[complete_path.size() - 1] == '/')
    {
        complete_path.resize(complete_path.size() - 1);
    }
    stat(complete_path.c_str(), &stat_buff);
    std::cerr << "the compleeete path: " << complete_path << std::endl;
    // std::cout << "value woould be: " << stat_buff.st_mode << " and val " << value << " to check " << S_ISDIR(stat_buff.st_mode) << std::endl;
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
    // std::cout << "the location is: " << _location_index << std::endl;
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

// this function will be able to use response, where it will build a http message
int Request::ft_http_status(int value)
{
    if (value < 300 || value == 301)
        return 1;
    if (!_parse->serv[_server_index]->error_num.empty())
    {
        for (size_t i = 0; i < _parse->serv[_server_index]->error_num.size(); ++i)
        {
            if (_parse->serv[_server_index]->error_num[i] == value)
            {
                _response_body_as_string = read_file(_parse->serv[_server_index]->error_page[i]);
                return value;
            }
        }
    }
    _response_body_as_string = "<!DOCTYPE html><html><body>\n<h2>";
    std::ostringstream converted;
    converted << this->getHttpStatus();
    _response_body_as_string.append(converted.str());
    _response_body_as_string.append("</h2>\n<h3>" + http_code[this->getHttpStatus()]);
    _response_body_as_string.append("</h3></body></html>");
    return this->getHttpStatus();
}

// std::string Request::read_file(std::string file)
// {
//     std::ifstream read_file;
//     std::string line;
//     std::string final_output;

//     read_file.open(file.c_str());
//     if (read_file.is_open())
//     {
//         while (read_file)
//         {
//             std::getline(read_file, line);
//             final_output.append(line);
//         }
//     }
//     else
//     {
//         perror("");
//         return std::string("");
//     }
//     return final_output;
// }

std::string Request::read_file(std::string file) {
    int fd_file = open(file.c_str(), O_RDONLY);
    int size_of_read = 1;
    char buffer_tmp[BUFFER_SIZE];
    std::string final_output;

    _final_file_size = 0;
    if (fd_file != -1) {
        while (size_of_read > 0) {
            size_of_read = read(fd_file, buffer_tmp, BUFFER_SIZE);
            for (int i = 0; i < size_of_read; ++i) {
                final_output += buffer_tmp[i];
            }
            _final_file_size += size_of_read;
            std::cerr << "the size that it reads: " << size_of_read << std::endl;
        }
    }
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
}

int Request::is_available(std::string key, std::string value)
{
    int val = _header.count(key);

    if (val && !value.empty() && _header[key] != value)
        return 0;
    if (!val)
        return 0;
    return 1;
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

int Request::getFile_size() const {
    return _final_file_size;
}

void Request::setParse(s_parsing *parsed)
{
    this->_parse = parsed;
}

void    Request::setServer(Server serv) {
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

void Request::print_parse_vector()
{
    std::vector<std::string>::iterator vec_b = _parse->serv[_server_index]->server.begin();
    std::vector<std::string>::iterator vec_e = _parse->serv[_server_index]->server.end();

    for (; vec_b != vec_e; ++vec_b)
    {
        std::cout << "value: " << *vec_b << std::endl;
    }
    std::cout << _parse->serv[_server_index]->server_name << std::endl;
    std::cout << _parse->serv[_server_index]->max_client << std::endl;
    std::cout << _parse->serv[_server_index]->loc[_location_index]->url_location << std::endl;
}

void Request::build_autoindex_page()
{
    DIR *dir;
    struct dirent *files;
    std::string root;

    root = _parse->serv[_server_index]->loc[_location_index]->root_location;
    dir = opendir(_directory_path.c_str());
    _response_body_as_string = "<!DOCTYPE html><html><body>";
    // for "." and ".." directories, need to be added
    _response_body_as_string.append("<a href=\"./\">.</a><br>");
    _response_body_as_string.append("<a href=\"../\">..</a><br>");
    // this loop will add or dirctories and files available
    while ((files = readdir(dir)) != NULL)
    {
        if (strcmp(files->d_name, ".") && strcmp(files->d_name, ".."))
        {
            if (_directory_path.find(_current_directory) != std::string::npos)
            {
                // _response_body_as_string.append("<a href=\"" + _directory_path.substr(_current_directory.size(), _directory_path.size()) + "/");
                _response_body_as_string.append("<a href=\"" + _path);
            }
            else
                _response_body_as_string.append("<a href=\"/");
            _response_body_as_string.append(files->d_name);
            _response_body_as_string.append("\">");
            _response_body_as_string.append(files->d_name);
            _response_body_as_string.append("</a><br>");
        }
    }
    closedir(dir);
    _response_body_as_string.append("</body></html>");
}

int Request::read_body_request() {
    std::ofstream jojo("jamal.txt", std::ios_base::app);

    if (_header.find("Content-Length") != _header.end() && _header.find("Transfer-Encoding") != _header.end())
    {
        _http_status = 400;
        return ft_http_status(getHttpStatus());
    }
    else if (_header.find("Content-Length") != _header.end())
    {
        char buffer_chr[BUFFER_SIZE];
        memset(buffer_chr, 0, BUFFER_SIZE);
        int content_length_read = recv(_read_fd, buffer_chr, BUFFER_SIZE, 0);
        jojo.write(buffer_chr,content_length_read);
        std::cout << content_length_read << std::endl;
        // if (content_length_read)
        //     _body.append(std::string(buffer_chr)); /// to check if something goes wrong
        // i+=content_length_read;
        // if(content_length_read == 0)
        //     break;
        _content_actual_size -= content_length_read;
        if (_content_actual_size > 0)
            read_again = true;
        else
            read_again = false;
    }
    else if (_header.find("Transfer-Encoding") != _header.end())
    {
        std::cerr << "whaaaats noooooooow a haaamiiiid" << std::endl;
        post_transfer_encoding();
    }
    std::cerr << "the content size: " << _content_actual_size << std::endl;
    jojo.close();
    std::cerr << "the status of read_again: " << read_again << std::endl;
    // std::cerr << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-**" << _buffer << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
    return read_again;
}

int Request::POST_method()
{
    // std::cerr << "all seem to be nice" << std::endl;
    if (location_support_upload())
    {
        if (read_body_request())
            return 1;
        upload_post_request();
        // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        // std::cout << "|>>>>>>>>>>>>>>|" << getBody() << "|<<<<<<<<<<<|" << std::endl;
        // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    else
    {
        if (!get_request_resource())
        {
            if (this->get_resource_type() == DIRECTORY)
                return this->If_is_directory();
            else if (this->get_resource_type() == FILE)
                return this->If_is_file();
        }
        else
        {
            _http_status = 404;
            return ft_http_status(getHttpStatus());
        }
        // else if (location_support_upload())
        // {
        //     upload_post_request();
        // }
    }
    return 0;
}

void    Request::post_transfer_encoding() {
    
    char buffer_chr[BUFFER_SIZE];
    bool done = false;
    std::ofstream jojo("jamal.txt", std::ios_base::app);
    int size = 0;

    memset(buffer_chr, 0, BUFFER_SIZE);
    if (_content_actual_size == 0) {
    std::cerr << "shooouuuuuuld be heeere just onceee" << std::endl;
    _content_actual_size = _server.getFirstReadSize();
    for (int i = 0; i < _server.getFirstReadSize(); i++) {
        if (_server.getBuffer()[i] == '\r' 
                                && (++i < _server.getFirstReadSize() && _server.getBuffer()[i] == '\n')
                                && (++i < _server.getFirstReadSize() && buffer_chr[i] == '\r')
                                && (++i < _server.getFirstReadSize() && buffer_chr[i] == '\n')
                                && (++i < _server.getFirstReadSize() && _server.getBuffer()[i] == '0') )
                                // && (++i < _server.getFirstReadSize() && buffer_chr[i] == '\r')
                                // && (++i < _server.getFirstReadSize() && buffer_chr[i] == '\n'))
        {
            // std::cerr << "that's good: |" << buffer_chr[i] << "| " << i  << std::endl;
            // exit(0);
            read_again = false;
            done = true;
            break;
        }

    }
    }

    if (!done) {
    size = recv(_read_fd, buffer_chr, BUFFER_SIZE, 0);
    jojo.write(buffer_chr, size);
    _content_actual_size += size;
    for (int i = 0; i < size; i++) {
        if (buffer_chr[i] == '\r' && (++i < size && buffer_chr[i] == '\n')
                                && (++i < size && buffer_chr[i] == '\r')
                                && (++i < size && buffer_chr[i] == '\n')
                                && (++i < size && buffer_chr[i] == '0')
                                && (++i < size && buffer_chr[i] == '\r')
                                && (++i < size && buffer_chr[i] == '\n'))
        {
            // std::cerr << "that's good: |" << buffer_chr[i] << "| " << i  << std::endl;
            // exit(0);
            read_again = false;
            done = true;
            break;
        }
    }
    }
    if (!done) {
        read_again = true;
    }
    else {
        read_again = false;
    }
    jojo.close();
}

int Request::upload_post_request()
{

    std::ifstream jojo("jamal.txt");
    std::string str = "Content-Disposition";
    std::string line;
    std::string ext;
    char c;
    std::string rand_str = randomstring(10);
    size_t find = _body.find("Content-Type:");
    if (find != std::string::npos)
    {
        ext = _body.substr(find);
        size_t fnd = ext.find("/");
        if (fnd != std::string::npos)
        {
            ext = ext.substr(fnd + 1);
            size_t found = ext.find("\n");
            if (found != std::string::npos)
                ext = "." + ext.substr(0, found - 1);
            rand_str += ext;
        }
    }
    std::ofstream goku("./upload/" + rand_str);
    while (std::getline(jojo, line))
    {
        if (line.find(str) != std::string::npos)
        {
            // std::getline(jojo, line);
            // std::getline(jojo, line);
            // std::getline(jojo, line);
            // std::getline(jojo, line);
            while (jojo.get(c))
            {
                goku.put(c);
            }
            break;
        }
    }
    _http_status = 201;
    return ft_http_status(getHttpStatus());
}

bool Request::location_support_upload()
{
    if (_header.find("Content-Type") != _header.end())
    {
        if (_header.find("Content-Type")->second.find("multipart/form-data") != std::string::npos)
            return true;
    }
    return false;
}
int Request::If_is_file()
{
    if (is_location_has_cgi())
    {
        request_run_cgi();
    }
    else
    {
        _http_status = 403;
        return ft_http_status(getHttpStatus());
    }
    return (0);
}

int Request::If_is_directory()
{
    if (is_uri_has_backslash_in_end())
    {
        if (is_dir_has_index_files())
        {
            if (is_location_has_cgi())
                request_run_cgi();
            else
            {
                _http_status = 403;
                return ft_http_status(getHttpStatus());
            }
        }
        else
        {
            _http_status = 404; // to check after ???????????
            return ft_http_status(getHttpStatus());
        }
    }
    else
    {
        _http_status = 301;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

bool Request::is_location_has_cgi()
{
    if (this->_parse->serv[_server_index]->loc[_location_index]->cgi_pass.size())
    {
        return true;
    }
    return false;
}

int Request::request_run_cgi()
{
    // Server server;
    int cgi_return = 1;
    CGI cgi(_location_index, _server_index);

    if (is_body_size_good(_server.getBuffer()))
    {
        _http_status = 413;
        return ft_http_status(getHttpStatus());
    }
    cgi_return = cgi.handle_cgi_request(*this, get_server_buffer().c_str(), _parse->serv[_server_index]);
    if (!cgi_return)
    {
        _response_body_as_string = cgi.getRespBuffer();
        return (200);
    }
    std::cout << "|____________|" << _response_body_as_string << "|_________|" << cgi_return << std::endl;
    _http_status = cgi_return;
    return ft_http_status(getHttpStatus());
}

std::string const &Request::getBody() const
{
    return _body;
}

void    Request::set_read_fd(int fd) {
    _read_fd = fd;
}

unsigned long   Request::hex_to_dec(std::string hex) {
    unsigned long output = 0;
    for (size_t i = 0; i < hex.length(); ++i) {
        if (hex[i] >= 48 && hex[i] <= 57) {
            output += (hex[i] - 48)* std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70) {
            output += (hex[i] - 55)* std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102) {
            output += (hex[i] - 87)* std::pow(16, hex.length() - i - 1);
        }
    }
    return output;
}

int     Request::string_to_decimal(std::string str) {
    std::stringstream converted;
    int               number;

    converted << str;
    converted >> number;

    return number;
}

void Request::set_cookie()
{
    // std::map<std::string, std::string>;
    // size_t find = _header.find("")

    std::map<std::string, std::string>::iterator it = _header.begin();
    // _cookie["set-cookie:"] =
    std::cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
    for (; it != _header.end(); it++)
    {
        // if
        std::cout << "  |>>>>>>>>>>>>>>|    " << it->first << "    |<<<<<<<<<<<|   " << it->second << std::endl;
    }

    std::cout << " |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| " << std::endl;
}
