/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 13:53:42 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/06/17 13:53:57 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::build_response()
{
    std::ostringstream converted;

    converted << this->getHttpStatus();
    Response = "HTTP/1.1 " + converted.str() + " " + http_code[this->getHttpStatus()] + "\r\n";
    std::string file_type;

    int position_extension = _available_file_path.find_last_of(".");
    file_type = _available_file_path.substr(position_extension + 1, _available_file_path.size());
    if (!file_type.empty() && _response_final.find("Content-Type") == _response_final.end())
        _response_final["Content-Type"] = mime_type[file_type];
    if (this->getHttpStatus() == 301)
    {
        if (this->_path[_path.size() - 1] == '/')
            _response_final["Location"] = _arguments["Host"] + this->_path;
        else
            _response_final["Location"] = _arguments["Host"] + this->_path + "/";
    }
    converted.str("");
    converted.clear();
    converted << _content_length;
    _response_final["Connection"] = "closed";
    if (_content_length)
        _response_final["Content-Length"] = converted.str();


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
    // Example:   Date: Thu 20 Apr 2023 01:22:10 GMT
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

// this function will be able to use response, where it will build a http message
int Request::ft_http_status(int value)
{
    _http_status = value;
    if (value < 300 || value == 301)
        return 2;
    if (!_parse->serv[_server_index]->error_num.empty())
    {
        for (size_t i = 0; i < _parse->serv[_server_index]->error_num.size(); ++i)
        {
            if (_parse->serv[_server_index]->error_num[i] == value)
            {
                std::string path = "";
                if (_parse->serv[_server_index]->error_page[i][0] == '/')
                    path = _current_directory + _parse->serv[_server_index]->error_page[i];
                else
                    path = _current_directory + '/' + _parse->serv[_server_index]->error_page[i];
                _response_body_as_string = read_file(path);
                if (_response_body_as_string == "")
                    break;
                return value;
            }
        }
    }
    _response_body_as_string = "<!DOCTYPE html><html><body>\n<h2><center>";
    std::ostringstream converted;
    converted << this->getHttpStatus();
    _response_body_as_string.append(converted.str());
    _response_body_as_string.append(" - " + http_code[this->getHttpStatus()]);
    _response_body_as_string.append("</center></h2><hr><h4><center>webserv</center></h4></body></html>");
    return this->getHttpStatus();
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

void Request::build_autoindex_page()
{
    DIR *dir;
    struct dirent *files;
    std::string root;

    root = _parse->serv[_server_index]->loc[_location_index]->root_location;
    dir = opendir(_directory_path.c_str());
    _response_body_as_string = "<!DOCTYPE html><html><body>";
    // for "." and ".." directories, need to be added
    _response_body_as_string.append("<h1>Index of " + _path + ":</h1><hr>");
    _response_body_as_string.append("<a href=\"./\">.</a><br>");
    _response_body_as_string.append("<a href=\"../\">..</a><br>");
    // this loop will add or dirctories and files available
    while ((files = readdir(dir)) != NULL)
    {
        if (strcmp(files->d_name, ".") && strcmp(files->d_name, ".."))
        {
            if (_directory_path.find(_current_directory) != std::string::npos)
                _response_body_as_string.append("<a href=\"" + _path);
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