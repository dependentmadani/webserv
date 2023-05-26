/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/05/26 10:08:54 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "cgi.hpp"
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <regex>

CGI::CGI(int loc_index, int serv_index) : _location_index(loc_index), _server_index(serv_index)
{
}

CGI::~CGI()
{
}
std::string CGI::trim(const std::string &str)
{
    std::regex pattern("^\\s+|\\s+$");

    return std::regex_replace(str, pattern, "");
}
void CGI::fill_env(std::string buffer)
{
    std::stringstream ss(buffer);
    std::string token;

    while (getline(ss, token, '\n'))
    {
        if (token != "\0")
        {
            hold.push_back(token);
        }
    }
    for (size_t j = 0; j < hold.size(); j++)
    {
        size_t found = hold[j].find("HTTP/");
        if (found != std::string::npos)
        {
            std::string str;
            found = hold[j].find("?");
            if (found != std::string::npos)
            {
                str = "QUERY_STRING=";
                str += hold[j].substr(found + 1);
                char *s = strtok(const_cast<char *>(str.c_str()), " ");
                _envcgi.push_back(s);
            }
            found = hold[j].find("GET");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char *>(hold[j].c_str()), " ");
                str += s;
                _envcgi.push_back(str);
            }
            found = hold[j].find("POST");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char *>(hold[j].c_str()), " ");
                str += s;
                _envcgi.push_back(str);
            }
            found = hold[j].find("DELETE");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char *>(hold[j].c_str()), " ");
                str += s;
                _envcgi.push_back(str);
            }

            found = hold[j].find("DELETE");
            if (found != std::string::npos)
            {
                str = "REQUEST_METHOD=";
                char *s = strtok(const_cast<char *>(hold[j].c_str()), " ");
                str += s;
                _envcgi.push_back(str);
            }
        }
    }
}

void CGI::fill_cgi(std::map<std::string, std::string> header, std::string buffer, t_server *serv)
{
    fill_env(buffer);
    std::string s = "SCRIPT_FILENAME=";
    s += _script_name;
    _envcgi.push_back(s);
    _envcgi.push_back("REDIRECT_STATUS=200");
    std::map<std::string, std::string>::iterator it = header.begin();
    while (it != header.end())
    {
        if (it->first == "Content-Type")
            _envcgi.push_back("CONTENT_TYPE=" + trim(it->second));
        if (it->first == "Content-Length")
            _envcgi.push_back("CONTENT_LENGTH=" + trim(it->second));
        if (it->first == "Cookie")
            _envcgi.push_back("HTTP_COOKIE=" + trim(it->second));
        it++;
    }
    _env = new char *[_envcgi.size() + 1];
    for (size_t i = 0; i < _envcgi.size(); i++)
        _env[i] = (strdup(_envcgi[i].c_str()));
    _env[_envcgi.size()] = NULL;
    check_cgi(serv->loc[_location_index]->cgi_pass);
}

int CGI::handle_cgi_request(Request &req, char const *buffer, t_server *serv)
{
    _script_name = req.getAvailableFilePath();
    char **ptr = new char *[3];
    ptr[0] = const_cast<char *>(executable.c_str());
    ptr[1] = const_cast<char *>(_script_name.c_str());
    ptr[2] = NULL;

    std::ifstream jojo("jamal.txt");
    std::ofstream goku("file.txt");
    std::string fileContent = readFileToString("jamal.txt");
    std::string substring = "\r\n\r\n";
    size_t position = fileContent.find(substring);
    std::string head = fileContent.substr(0, position);
    for (size_t i = position + 4; i < fileContent.size(); i++)
    {
        goku.put(fileContent[i]);
    }
    goku.close();
    fill_cgi(req.getHeader(), head, serv);
    if (!executable.size())
    {
        std::ifstream file(_script_name.c_str());
        std::string content;
        if (file.is_open())
        {
            content.assign((std::istreambuf_iterator<char>(file)),
                           (std::istreambuf_iterator<char>()));
            file.close();
        }
        resp_buffer = content;
        return 1;
    }

    for (size_t i = 0; i < _envcgi.size(); i++)
    {
        std::cout << i << "|++++++++++|+++++÷++|" << _env[i] << "|+++++++++|++++++++++++|" << strlen(_env[i]) << std::endl;
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        int fd = open("file.txt", O_RDONLY);
        int out_fd = open("./out_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        // std::cout << "=> " << out_fd << " " << fd << std::endl;
        dup2(fd, STDIN_FILENO);
        // close(fd);
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        // char *env[] = {
        //     "CONTENT_LENGTH=87864",
        //     "CONTENT_TYPE=multipart/form-data; boundary=---------------------------290948205715912812062873960758",
        //     "REQUEST_METHOD=POST",
        //     "SCRIPT_FILENAME=/Users/sriyani/Desktop/goku/public/script/php_cgi.php",
        //     "REDIRECT_STATUS=200"};

        execve(ptr[0], ptr, _env);
        std::cerr << "Error executing CGI script" << std::endl;
        exit(1);
    }
    else
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
            perror("wait() error");
    }
    size_t rd;
    char bufffer[4096] = " ";
    int out_fd = open("./out_result.txt", O_RDONLY);
    while ((rd = read(out_fd, bufffer, sizeof(buffer))) > 0)
    {
        resp_buffer += bufffer;
    }
    close(out_fd);
    size_t found = 0;
    if (_ext == ".pl")
        found = resp_buffer.find("\n\n");
    else
        found = resp_buffer.find("\r\n\r\n");
    hold_ContentType = resp_buffer.substr(0, found);
    size_t fnd = hold_ContentType.find("Content-type:");
    if (fnd != std::string::npos)
        resp_buffer = resp_buffer.substr(found + 1);
    return (0);
}

std::string const &CGI::getRespBuffer() const
{
    return resp_buffer;
}

std::string const &CGI::getContentType() const
{
    return hold_ContentType;
}

void CGI::check_cgi(std::vector<std::string> str)
{
    std::vector<std::string>::iterator it = str.begin();
    std::string hold = _script_name;
    _ext = hold.erase(0, hold.find_last_of('.'));
    int i = 0;
    while (it != str.end())
    {
        if (*it == _ext.c_str() + 1)
        {
            executable = str[i + 1];
            break;
        }
        ++it;
        ++i;
    }
}
