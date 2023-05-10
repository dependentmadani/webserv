/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/10 14:52:22 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../cgi-bin/cgi.hpp"
Request::Request() : _directory_path(), _method(), _path(), _arguments(), _protocol() ,_response(), http_code(), allowed_methods()
{
    _location_index = 0;
    _http_status = 200;
    _content_length = 0;
}

Request::~Request()
{ }

int     Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');

    this->FirstLinerRequest(splited_request[0]);
    this->HeaderRequest(request_message);
    this->get_location_index();
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
    // print_parse_vector();
    return 0;
}

void    Request::get_location_index() {
    for (int i = 0; i < _parse->serv[0]->num_location; ++i)
    {
        int size_for_path = _parse->serv[0]->loc[i]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[0]->loc[i]->url_location.size();
        if (this->getPath().substr(0, size_for_path) == _parse->serv[0]->loc[i]->url_location)
            _location_index = i;
    }
}

int     Request::UseMethod()
{
    if (_method == "GET")
        this->GET_method();
    else if (_method == "POST")
        this->POST_method();
    else if (_method == "DELETE")
        this->DELETE_method();
    return 0;
}

void    Request::build_response()
{
    std::ostringstream converted;

    converted << this->getHttpStatus();
    // _response["http_version"] = "HTTP/1.1";
    // _response["return_code"] = converted.str();
    // _response["status"] = http_code[this->getHttpStatus()];
    Response = "HTTP/1.1 " + converted.str() + " " + http_code[this->getHttpStatus()] + "\r\n";
    // _response_final["Location"] = "http://" + _parse->serv[0]->server_name +_path; // to make a variable to contruct the full location
    std::string file_type;

    int position_extension = _available_file_path.find_last_of(".");
    file_type = _available_file_path.substr(position_extension + 1, _available_file_path.size());
    if (!file_type.empty())
        _response_final["Content_Type"] = mime_type[file_type];
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

void Request::add_zero(int timer) {
    if (timer >= 0 && timer < 10) {
        Response.append("0");
    }
}

void Request::build_date() {
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
    if (this->get_request_resource() && !_parse->serv[0]->loc[_location_index]->auto_index)
    {
        _http_status = 404;
        return this->ft_http_status(getHttpStatus());
    }
    if (this->get_resource_type() == DIRECTORY)
        return this->Is_directory();
    else if (this->get_resource_type() == FILE)
        return this->Is_file();
    return 1;
}

int Request::get_request_resource()
{
    struct stat stat_buff;

    for (std::vector<std::string>::iterator b = _file_name_path.begin(); b != _file_name_path.end() ; ++b)
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

int     Request::if_location_has_cgi()
{
    if (_parse->serv[0]->loc[_location_index]->cgi_pass.empty())
    {
        this->Is_file();
        _http_status = 200;
        ft_http_status(getHttpStatus());
        return 0;
    }
    //call the constructor of cgi, than get the data from cgi. All of that as an else condition
    CGI();
    _http_status = 200; //to check depends on cgi
    ft_http_status(getHttpStatus());
    return 1;
}

int    Request::Is_directory()
{
    if (is_uri_has_backslash_in_end())
    {
        if ( !is_dir_has_index_files() )
        {
            if ( !get_auto_index() )
            {
                _http_status = 403;
                return ft_http_status(getHttpStatus());
            }
            else
            {
                //build an autoindex page in response.
                this->build_autoindex_page();
                _http_status = 200;
                return ft_http_status(getHttpStatus());
            }
        }
        else
        {
            //if this directory has an index file, it should check for cgi in location
            return this->if_location_has_cgi();
        }
    }
    else
    {
        if ( !get_auto_index() )
        {
            _http_status = 403;
            return ft_http_status(getHttpStatus());
        }
        //redirect the request by adding "/" to the request path.
        this->build_autoindex_page();
        _http_status = 301;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int     Request::is_uri_has_backslash_in_end()
{
    if (_path[_path.size() - 1] == '/')
        return 1;
    return 0;
}

int     Request::is_dir_has_index_files()
{
    for (int i = 0; i < _parse->serv[0]->num_location; ++i)
    {
        int size_for_path = _parse->serv[0]->loc[i]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[0]->loc[i]->url_location.size();
        if (this->getPath().substr(0, size_for_path) == _parse->serv[0]->loc[i]->url_location)
        {
            if (!_parse->serv[0]->loc[i]->index.empty())
                return 1;
        }
    }
    return 0;
}

bool Request::get_auto_index()
{
    // for (int i = 0; i < _parse->serv[0]->num_location; ++i)
    // {
        int size_for_path = _parse->serv[0]->loc[_location_index]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[0]->loc[_location_index]->url_location.size();
        if (this->getPath().substr(0, size_for_path) == _parse->serv[0]->loc[_location_index]->url_location)
        {
            return _parse->serv[0]->loc[_location_index]->auto_index;
        }
    // }
    return false;
}

int    Request::Is_file()
{
    std::ifstream   file;
    std::string     line;

    file.open(_available_file_path.c_str());
    if (!_parse->serv[0]->loc[_location_index]->cgi_pass.empty())
        return this->if_location_has_cgi();
    if (file.is_open())
    {
        while(file)
        {
            std::getline(file, line);
            _response_body_as_string.append(line);
        }
        // close(file);
    }
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

int    Request::Is_directory_for_DELETE()
{
    if (is_uri_has_backslash_in_end())
    {
        if ( !if_location_has_cgi() )
        {
            if ( is_dir_has_index_files() )
            {
                //run cgi on requested file with DELETE REQUEST METHOD
                //and check if this directory has an index file and cgi
                //then return code depend on cgi
            }
            else
            {
                //build an autoindex page in response.
                _http_status = 403;
                return ft_http_status(getHttpStatus());
            }
        }
        else
        {
            //delete all folder content
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
        //redirect the request by adding "/" to the request path.
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
        //nothing to do here for the moment. waiting for cgi to be done.
        //return code depend on cgi
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

    if ((dir = opendir(folder_file.c_str())) != NULL) {
      /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {

            if ((!strcmp(ent->d_name, ".") || strcmp(ent->d_name, "..")) && (strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))) {
                std::string tmp;
                if (type == DIRECTORY)
                    tmp = folder_file + "/" + ent->d_name;
                else
                    tmp = folder_file;
                std::cout << "the file is: " << tmp << std::endl;
                std::cout << "the file would be: " << ent->d_name << std::endl;
                if (!std::remove(tmp.c_str())) {
                    std::cout << "well removed file" << std::endl;
                }
                else {
                    std::cout << "something wrong with file: " << ent->d_name << std::endl;
                    return 0;
                }
            }
        }
        closedir (dir);
    }
    else if (type == FILE)
    {
        std::cout << "I believe it reached here: " << folder_file << std::endl;
        if (!std::remove(folder_file.c_str())) {
            std::cout << "well removed the file" << std::endl;
        }
        else {
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
    if ( check_access != 0 )
        return 0;
    return 1;
}

int    Request::FirstLinerRequest(char *request_message)
{
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

int     Request::check_for_arguments_in_path(std::string path) {
    size_t position = path.find("?");
    if (position != std::string::npos) {
        char *catcher = strtok((char *)path.c_str(), "?");
        _path = std::string(catcher);
        if ((position + 1) == path.size())
            return 1;
        catcher = strtok(NULL, "?");
        std::string args = std::string(catcher);
        char** splited_args = ft_split(args.c_str(), '&');
        int i = 0;
        while (splited_args[i]) {
            std::string tmp = splited_args[i];
            size_t pos = tmp.find("=");
            _arguments[tmp.substr(0, pos)] = tmp.substr(pos + 1, tmp.size());
            i++;
        }
    }
    else {
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

int    Request::HeaderRequest(char *request_message)
{
    char **splited_header = ft_split(request_message, '\n');
    // char *tmp = request_message;
    // char *tmp_request = request_message;
    // char *splited_header = strtok_r(tmp_request, "\r\n", &tmp);

    // splited_header = strtok_r(NULL, "\r\n", &tmp);
    int i = 1;
    while (splited_header[i] != NULL)
    {
        if (std::string(splited_header[i]).find(":") != std::string::npos)
        {
            char **split_each_line = ft_split(splited_header[i], ':');
            _response[std::string(split_each_line[0])] = std::string(split_each_line[1]);
            i++;
        }
        else
            break ;
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
    //check if path is valid, for security reason
    int path_counter = 0;
    size_t pos = 0;
    std::string url = getPath();

    url.erase(0, 1);
    std::string tmp;
    while ( ( pos = url.find("/") ) != std::string::npos)
    {
        tmp = url.substr(0, pos);
        // path_counter = ( (tmp == "..") ? --path_counter : ++path_counter );
        if (tmp == "..")
            --path_counter;
        else
            ++path_counter;
        if (path_counter < 0)
        {
            _http_status = 400;
            return ft_http_status(getHttpStatus());
        }
        url.erase(0, pos + 1);
    }
    //need to check if its available
    size_t number_of_location = 2;
    bool check_availability = false;
    for (size_t i = 0; i < number_of_location; ++i)
    {
        int size_for_path = _parse->serv[0]->loc[i]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[0]->loc[i]->url_location.size();
        if (this->getPath().substr(0, size_for_path) == _parse->serv[0]->loc[i]->url_location)
            check_availability = true;
    }
    if (!check_availability)
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int Request::is_location_have_redirection()
{
    for (size_t i = 0; i < _parse->serv[0]->loc[_location_index]->location.size(); ++i)
    {
        if (_parse->serv[0]->loc[_location_index]->location[i].find("return 301") != std::string::npos)
        {
            size_t position_of_return = _parse->serv[0]->loc[_location_index]->location[i].find("return 301") + 10;
            _http_status = 301;
            _path = _parse->serv[0]->loc[_location_index]->location[i].substr(position_of_return, _parse->serv[0]->loc[_location_index]->location[i].size());
            _path = remove_space(_path);
            return ft_http_status(getHttpStatus());
        }
    }
    return 0;
}

int Request::is_method_allowed_in_location()
{
    int size_for_path = _parse->serv[0]->loc[_location_index]->url_location.size() > getPath().size()? getPath().size() : _parse->serv[0]->loc[_location_index]->url_location.size();
    if (this->getPath().substr(0, size_for_path) == _parse->serv[0]->loc[_location_index]->url_location)
    {
        for (size_t i = 0; i < _parse->serv[0]->loc[_location_index]->methods.size(); ++i)
        {
            if (this->getMethod() == _parse->serv[0]->loc[_location_index]->methods[i])
                return 0;
        }
    }
    return 1;
}

void    Request::reform_requestPath_locationPath()
{
    std::string get_root;
    struct stat stat_buff;
    int     i = 0;

    i = _location_index;
    if (this->getPath()[_parse->serv[0]->loc[i]->url_location.size()] == '/' && this->getPath().size() != 1)
        get_root = this->getPath().substr(_parse->serv[0]->loc[i]->url_location.size() + 1, this->getPath().size());
    else if (this->getPath().size() == 1)
        get_root = this->getPath();
    else
        get_root = this->getPath().substr(_parse->serv[0]->loc[i]->url_location.size(), this->getPath().size());
    while (get_root[get_root.size() - 1] == '/' && this->getPath().size() != 1)
    {
        get_root.resize(get_root.size() - 1);
    }
    std::string complete_path = _parse->serv[0]->loc[i]->root_location + "/" + get_root;
    while (complete_path[complete_path.size() - 1] == '/')
    {
        complete_path.resize(complete_path.size() - 1);
    }
    stat(complete_path.c_str(), &stat_buff);
    // std::cout << "value woould be: " << stat_buff.st_mode << " and val " << value << " to check " << S_ISDIR(stat_buff.st_mode) << std::endl;
    if (S_ISDIR(stat_buff.st_mode))
    {
        std::cout << "complete path: " << complete_path << std::endl;
        _directory_path = complete_path;
        _file_directory_check = DIRECTORY;
        if (!_parse->serv[0]->loc[i]->index.empty())
        {
            for (size_t index_indexes = 0; index_indexes < _parse->serv[0]->loc[i]->index.size(); ++index_indexes)
            {
                _file_name_path.push_back(complete_path + "/" + _parse->serv[0]->loc[i]->index[index_indexes]);
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
        _http_status = 403; //to check the http status code after...!!
        return ft_http_status(this->getHttpStatus());
    }
    _http_status = 501;
    return ft_http_status(this->getHttpStatus());
}

//this function will be able to use response, where it will build a http message
int Request::ft_http_status(int value)
{
    if (value < 300 || value == 301)
        return 1;
    if (!_parse->serv[0]->error_num.empty()) {
    for (size_t i = 0; i < _parse->serv[0]->error_num.size(); ++i) {
        
        if (_parse->serv[0]->error_num[i] == value)
        {
            _response_body_as_string = read_file(_parse->serv[0]->error_page[i]);
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

std::string Request::read_file(std::string file)
{
    std::ifstream   read_file;
    std::string     line;
    std::string     final_output;

    read_file.open(file.c_str());
    if (read_file.is_open())
    {
        while(read_file)
        {
            std::getline(read_file, line);
            final_output.append(line);
        }
    }
    else {
        perror("");
        return std::string("");
    }
    return final_output;
}

void    Request::ft_mime_type()
{
    //text
    mime_type["php"]    = "text/html";
    mime_type["html"]   = "text/html";
    mime_type["htm"]    = "text/html";
    mime_type["shtml"]  = "text/html";
    mime_type["css"]    = "text/css";
    mime_type["xml"]    = "text/xml";
    //image 
    mime_type["gif"]    = "image/gif";
    mime_type["jpeg"]   = "image/jpeg";
    mime_type["jpg"]    = "image/jpeg";
    //application 
    mime_type["js"]     = "application/javascript";
    mime_type["atom"]   = "application/atom+xml";
    mime_type["rss"]    = "application/rss+xml";
    //text 
    mime_type["mml"]    = "text/mathml";
    mime_type["txt"]    = "text/plain";
    mime_type["jad"]    = "text/vnd.sun.j2me.app-descriptor";
    mime_type["wml"]    = "text/vnd.wap.wml";
    mime_type["htc"]    = "text/x-component";
    //image 
    mime_type["avif"]   = "image/avif";
    mime_type["png"]    = "image/png";
    mime_type["svg"]    = "image/svg+xml";
    mime_type["svgz"]   = "image/svg+xml";
    mime_type["tif"]    = "image/tiff";
    mime_type["tiff"]   = "image/tiff";
    mime_type["wbmp"]   = "image/vnd.wap.wbmp";
    mime_type["webp"]   = "image/webp";
    mime_type["ico"]    = "image/x-icon";
    mime_type["jng"]    = "image/x-jng";
    mime_type["bmp"]    = "image/x-ms-bmp";
    //font 
    mime_type["woff"]   = "font/woff";
    mime_type["woff2"]  = "font/woff2";
    //application 
    mime_type["jar"]    = "application/java-archive";
    mime_type["war"]    = "application/java-archive";
    mime_type["ear"]    = "application/java-archive";
    mime_type["json"]   = "application/json";
    mime_type["hqx"]    = "application/mac-binhex40";
    mime_type["doc"]    = "application/msword";
    mime_type["pdf"]    = "application/pdf";
    mime_type["ps"]     = "application/postscript";
    mime_type["eps"]    = "application/postscript";
    mime_type["ai"]     = "application/postscript";
    mime_type["rtf"]    = "application/rtf";
    mime_type["m3u8"]   = "application/vnd.apple.mpegurl";
    mime_type["kml"]    = "application/vnd.google-earth.kml+xml";
    mime_type["kmz"]    = "application/vnd.google-earth.kmz";
    mime_type["xls"]    = "application/vnd.ms-excel";
    mime_type["eot"]    = "application/vnd.ms-fontobject";
    mime_type["ppt"]    = "application/vnd.ms-powerpoint";
    mime_type["odg"]    = "application/vnd.oasis.opendocument.graphics";
    mime_type["odp"]    = "application/vnd.oasis.opendocument.presentation";
    mime_type["ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
    mime_type["odt"]    = "application/vnd.oasis.opendocument.text";
    mime_type["pptx"]   = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime_type["xlsx"]   = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_type["docx"]   = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime_type["wmlc"]   = "application/vnd.wap.wmlc";
    mime_type["wasm"]   = "application/wasm";
    mime_type["7z"]     = "application/x-7z-compressed";
    mime_type["cco"]    = "application/x-cocoa";
    mime_type["jardiff"]= "application/x-java-archive-diff";
    mime_type["jnlp"]   = "application/x-java-jnlp-file";
    mime_type["run"]    = "application/x-makeself";
    mime_type["pl"]     = "application/x-perl";
    mime_type["pm"]     = "application/x-perl";
    mime_type["prc"]    = "application/x-pilot";
    mime_type["pdb"]    = "application/x-pilot";
    mime_type["rar"]    = "application/x-rar-compressed";
    mime_type["rpm"]    = "application/x-redhat-package-manager";
    mime_type["sea"]    = "application/x-sea";
    mime_type["swf"]    = "application/x-shockwave-flash";
    mime_type["sit"]    = "application/x-stuffit";
    mime_type["tcl"]    = "application/x-tcl";
    mime_type["tk"]     = "application/x-tcl";
    mime_type["der"]    = "application/x-x509-ca-cert";
    mime_type["pem"]    = "application/x-x509-ca-cert";
    mime_type["crt"]    = "application/x-x509-ca-cert";
    mime_type["xpi"]    = "application/x-xpinstall";
    mime_type["xhtml"]  = "application/xhtml+xml";
    mime_type["xspf"]   = "application/xspf+xml";
    mime_type["zip"]    = "application/zip";
    mime_type["bin"]    = "application/octet-stream";
    mime_type["exe"]    = "application/octet-stream";
    mime_type["dll"]    = "application/octet-stream";
    mime_type["deb"]    = "application/octet-stream";
    mime_type["dmg"]    = "application/octet-stream";
    mime_type["iso"]    = "application/octet-stream";
    mime_type["img"]    = "application/octet-stream";
    mime_type["msi"]    = "application/octet-stream";
    mime_type["msp"]    = "application/octet-stream";
    mime_type["msm"]    = "application/octet-stream";
    //audio
    mime_type["mid"]    = "audio/midi";
    mime_type["midi"]   = "audio/midi";
    mime_type["kar"]    = "audio/midi";
    mime_type["mp3"]    = "audio/mpeg";
    mime_type["ogg"]    = "audio/ogg";
    mime_type["m4a"]    = "audio/x-m4a";
    mime_type["ra"]     = "audio/x-realaudio";
    //video
    mime_type["3gpp"]   = "video/3gpp";
    mime_type["3gp"]    = "video/3gpp";
    mime_type["ts"]     = "video/mp2t";
    mime_type["mp4"]    = "video/mp4";
    mime_type["mpeg"]   = "video/mpeg";
    mime_type["mpg"]    = "video/mpeg";
    mime_type["mov"]    = "video/quicktime";
    mime_type["webm"]   = "video/webm";
    mime_type["flv"]    = "video/x-flv";
    mime_type["m4v"]    = "video/x-m4v";
    mime_type["mng"]    = "video/x-mng";
    mime_type["asx"]    = "video/x-ms-asf";
    mime_type["asf"]    = "video/x-ms-asf";
    mime_type["wmv"]    = "video/x-ms-wmv";
    mime_type["avi"]    = "video/x-msvideo";
}

void    Request::ft_http_code()
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
    int val = _response.count(key);

    if (val && !value.empty() && _response[key] != value)
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
    char *tmp_request_body = strstr(request_message, "\r\n\r\n");
    tmp_request_body[0] = '\0';
    _body = std::string(tmp_request_body + 4);
    if (_body.size() > (size_t)_parse->serv[0]->max_client)
        return 1;
    return 0;
}

std::string Request::getPath() const
{
    return _path;
}

int     Request::getHttpStatus() const
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

void    Request::setParse(s_parsing* parsed)
{
    this->_parse = parsed;
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

void    Request::print_parse_vector()
{
    std::vector<std::string>::iterator vec_b = _parse->serv[0]->server.begin();
    std::vector<std::string>::iterator vec_e = _parse->serv[0]->server.end();

    for (; vec_b != vec_e; ++vec_b)
    {
        std::cout << "value: " << *vec_b << std::endl;
    }
    std::cout << _parse->serv[0]->server_name << std::endl;
    std::cout << _parse->serv[0]->max_client << std::endl;
    std::cout << _parse->serv[0]->loc[_location_index]->url_location << std::endl;
}

void    Request::build_autoindex_page() {
    DIR *dir;
    struct dirent *files;

    dir = opendir(_directory_path.c_str());
    _response_body_as_string = "<!DOCTYPE html><html><body>";
    //for "." and ".." directories, need to be added
    _response_body_as_string.append("<a href=\"" + _directory_path + "/.\">.</a><br>");
    _response_body_as_string.append("<a href=\"" + _directory_path + "/..\">..</a><br>");
    //this loop will add or dirctories and files available
    while ((files = readdir(dir)) != NULL)
    {
        if (strcmp(files->d_name, ".") && strcmp(files->d_name, "..") ) {
        _response_body_as_string.append("<a href=\"" + _directory_path + "/");
        _response_body_as_string.append(files->d_name);
        _response_body_as_string.append("\">");
        _response_body_as_string.append(files->d_name);
        _response_body_as_string.append("</a><br>");
        }
    }
    closedir(dir);
    _response_body_as_string.append("</body></html>");
}

int Request::POST_method()
{
    if (location_support_upload()) 
        upload_post_request();
    else
    {
        if (get_request_resource())
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
    }
    return 0;
}
int Request::upload_post_request()
{

    _http_status = 201;
    return ft_http_status(getHttpStatus());
}

bool Request::location_support_upload()
{
    std::string value = _response["Content-Type"];
    size_t find = value.find("multipart/form-data");
    if (find != std::string::npos)
        return true;
    return false;
}
int Request::If_is_file()
{
    if (is_location_has_cgi())
        request_post_run_cgi();
    else
    {
        _http_status = 403;
        return ft_http_status(getHttpStatus()); 
    }
    return (0);
}


int    Request::If_is_directory()
{
    if (is_uri_has_backslash_in_end())
    {
        if ( is_dir_has_index_files() )
        {
            if (is_location_has_cgi())
                request_post_run_cgi();
            else
            {
                _http_status = 403;
                return ft_http_status(getHttpStatus()); 
            }
        }
        else
        {
            _http_status = 404; //to check after ???????????
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
    if (this->_parse->serv[0]->loc[_location_index]->cgi_pass.size())
    {
        return true;
    }
    return false;
}
int     Request::request_post_run_cgi()
{
    Server server;
    CGI cgi;
    // std::cerr << "|****************|" << std::endl;
    is_body_size_good(server.getBuffer());
    cgi.fill_cgi(server.getBuffer(), _parse->serv[0]);
    cgi.handle_cgi_request(*this);
    
    return (0);
}
