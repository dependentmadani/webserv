/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:30:30 by sriyani           #+#    #+#             */
/*   Updated: 2023/06/15 19:54:12 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <signal.h>

CGI::CGI(int loc_index, int serv_index) : _location_index(loc_index), _server_index(serv_index)
{
}

CGI::~CGI()
{
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

    std::ifstream in_file("temp_file");
    std::ofstream out_file("file.txt");
    std::string fileContent = readFileToString("temp_file");
    std::string substring = "\r\n\r\n";
    size_t position = fileContent.find(substring);
    std::string head = fileContent.substr(0, position);
    for (size_t i = position + 4; i < fileContent.size(); i++)
    {
        out_file.put(fileContent[i]);
    }
    out_file.close();
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
        for (size_t i = 0; i < _envcgi.size(); i++)
        {
            delete _env[i];
        }
        delete[] _env;
        return 1;
    }
    unlink("temp_file");
    pid_t pid = fork();
    char **ptr = new char *[3];
    ptr[0] = const_cast<char *>(executable.c_str());
    ptr[1] = const_cast<char *>(_script_name.c_str());
    ptr[2] = NULL;
    // int ala = 0;
    if (pid < 0)
    {
        std::cerr << "Error forking process" << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {

        int fd = open("file.txt", O_RDONLY);
        int out_fd = open("./out_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        dup2(fd, STDIN_FILENO);
        close(fd);
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        // ala = alarm(20);

        execve(ptr[0], ptr, _env);
        std::cerr << "Error executing CGI script" << std::endl;
        exit(1);
    }
    else
    {
        // try
        // {
        //     if (ala)
        //         throw ala;
        // }

        // catch (std::out_of_range &e)
        // {
        //     resp_buffer = "ERROR";
        //     std::cerr << "Out of range exception: " << e.what() << std::endl;
        //     exit(1);
        // }
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
    unlink("file.txt");
    unlink("out_result.txt");
    size_t found = 0;
    if (_ext == ".pl")
        found = resp_buffer.find("\n\n");
    else
        found = resp_buffer.find("\r\n\r\n");
    hold_ContentType = resp_buffer.substr(0, found);
    size_t fnd = hold_ContentType.find("Content-type:");
    if (fnd != std::string::npos)
        resp_buffer = resp_buffer.substr(found + 1);
    for (size_t i = 0; i < _envcgi.size(); i++)
    {
        delete _env[i];
    }
    delete[] _env;
    delete[] ptr;
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
