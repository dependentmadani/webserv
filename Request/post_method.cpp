/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:40:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/06/17 11:41:54 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

int Request::POST_method()
{
    int state = 0;
    //check if the directory where to upload to, is available
    if (_parse->serv[_server_index]->loc[_location_index]->uploads.empty()) {
        _http_status = 406;
        return ft_http_status(this->getHttpStatus());
    }
    if (_parse->serv[_server_index]->loc[_location_index]->uploads[0] == '/')
        _directory_to_upload_in = _current_directory + _parse->serv[_server_index]->loc[_location_index]->uploads;
    else
        _directory_to_upload_in = _current_directory + '/' + _parse->serv[_server_index]->loc[_location_index]->uploads;
    //check the condition of upload
    if (location_support_upload())
    {
        if (read_body_request())
        {
            return 1;
        }
        upload_post_request();
        state = 1;
        if (!_parse->serv[_server_index]->loc[_location_index]->cgi_pass.size())
            unlink("temp_file");
    }
    if (!get_request_resource())
    {
        if (this->get_resource_type() == DIRECTORY)
            return this->If_is_directory();
        else if (this->get_resource_type() == FILE)
            return this->If_is_file();
    }
    else if (!state)
    {
        _http_status = 404;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

void Request::post_transfer_encoding()
{

    char buffer_chr[BUFFER_SIZE];
    bool done = false;
    std::ofstream in_file("temp_file", std::ios_base::app);
    int size = 0;

    memset(buffer_chr, 0, BUFFER_SIZE);
    if (_content_actual_size == 0)
    {
        _content_actual_size = _server.getFirstReadSize();
        for (int i = 0; i < _server.getFirstReadSize(); i++)
        {
            if (_server.getBuffer()[i] == '\r' && (++i < _server.getFirstReadSize() 
            && _server.getBuffer()[i] == '\n') && (++i < _server.getFirstReadSize() 
            && buffer_chr[i] == '\r') && (++i < _server.getFirstReadSize() 
            && buffer_chr[i] == '\n') && (++i < _server.getFirstReadSize() && _server.getBuffer()[i] == '0'))
            {
                read_again = false;
                done = true;
                break;
            }
        }
    }

    if (!done)
    {
        size = recv(_read_fd, buffer_chr, BUFFER_SIZE, 0);
        in_file.write(buffer_chr, size);
        _content_actual_size += size;
        for (int i = 0; i < size; i++)
        {
            if (buffer_chr[i] == '\r' && (++i < size && buffer_chr[i] == '\n') 
            && (++i < size && buffer_chr[i] == '\r') && (++i < size && buffer_chr[i] == '\n') 
            && (++i < size && buffer_chr[i] == '0') && (++i < size && buffer_chr[i] == '\r') 
            && (++i < size && buffer_chr[i] == '\n'))
            {
                read_again = false;
                done = true;
                break;
            }
        }
    }
    if (!done)
    {
        read_again = true;
    }
    else
    {
        read_again = false;
    }
    in_file.close();
}

int Request::upload_post_request()
{

    std::ifstream in_file("temp_file");
    std::string str = "Content-Disposition";
    std::string line;
    std::string ext;
    // char c;
    std::string rand_str = randomstring(3);
    size_t find = _body.find("Content-Disposition");
    if (find != std::string::npos)
    {
        ext = _body.substr(find);
        size_t fnd = ext.find("filename=");
        if (fnd != std::string::npos)
        {
            ext = ext.substr(fnd + 2);
            size_t found = ext.find(".");
            if (found != std::string::npos)
                ext = ext.substr(found);
            size_t trv = ext.find("\"");
            ext = ext.substr(0, trv);
            rand_str += ext;
        }
    }
    else
    {
        ext = ".";
        std::string find = _header["Content-Type"];
        if (find.size())
        {
            size_t fnd = find.find("/");
            if (fnd != std::string::npos)
            {
                ext += find.substr(fnd + 1);
                ext = ext.substr(0, ext.length() - 1);
                rand_str += ext;
            }
        }
        else
        {
            _http_status = 400;
            return ft_http_status(getHttpStatus());
        }
    }
    std::ofstream out_file(_directory_to_upload_in + "/" + rand_str);

    if (_body.find("Content-Type") != std::string::npos)
    {
        std::string fileContent = readFileToString("temp_file");
        std::string substring = "\r\n\r\n";
        size_t position = fileContent.find(substring);
        std::string mybody = fileContent.substr(position + 4);
        size_t pos = mybody.find("\r\n\r\n");

        for (size_t i = pos + 4; i < mybody.size(); i++)
        {
            out_file.put(mybody[i]);
        }
    }
    else
    {
        std::string fileContent = readFileToString("temp_file");
        std::string substring = "\r\n\r\n";
        size_t position = fileContent.find(substring);
        _response_body_as_string = fileContent.substr(position + 4);
        for (size_t i = position + 4; i < fileContent.size(); i++)
        {
            out_file.put(fileContent[i]);
        }
    }

    _http_status = 201;
    return ft_http_status(getHttpStatus());
}

bool Request::location_support_upload()
{

    size_t find = _body.find("Content-Type");
    size_t found = _body.find("Content-Disposition");
    std::string fnd = _header["Content-Type"];
    std::string lent = _header["Content-Length"];
    int num = atoi(lent.c_str());
    if (find != std::string::npos || (found == std::string::npos && fnd.size() && num > 0))
    {
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
    int cgi_return = 1;
    CGI cgi(_location_index, _server_index);

    if (!finished) {
        char bufffer[BUFFER_SIZE] = " ";
        memset(bufffer, 0, BUFFER_SIZE);
        int rd = read(_fd_file, bufffer, sizeof(bufffer));
        if (rd > 0)
            Response = "";
            for (int i = 0; i < rd; ++i)
                Response += bufffer[i];
            _content_length = rd;
        if (rd <= 0) {
            finished = true;
        }
        if (_fd_file > 0 && finished) {
            close(_fd_file);
            _fd_file = 0;
            unlink("file.txt");
            unlink("out_result.txt");
        }
        _http_status = 200;
        return ft_http_status(getHttpStatus());
    }

    if (access(_available_file_path.c_str(), R_OK) != 0)
    {
        _http_status = 403;
        return ft_http_status(getHttpStatus());
    }
    if (is_body_size_good(_server.getBuffer()))
    {
        _http_status = 413;
        return ft_http_status(getHttpStatus());
    }
    cgi_return = cgi.handle_cgi_request(*this, get_server_buffer().c_str(), _parse->serv[_server_index]);
    if (!this->_check_cgi && cgi.get_checker())
    {
        this->_cgi_helper = cgi;
        this->_check_cgi = 1;
        return 1;
    }
    else if (this->_check_cgi && !_cgi_helper.get_checker()){
        this->_check_cgi = 0;
    }
    if (!cgi_return)
    {
        _response_body_as_string = cgi.getRespBuffer();
        if (cgi.getRespBuffer().size() > 13)
            _response_final["Content-Type"] = cgi.getContentType().substr(13, cgi.getContentType().size());
        if (_response_body_as_string.empty())
        {
            _response_body_as_string = "<!DOCTYPE html>"
                            "<html>"
                            "<body>"
                            "</body>"
                            "</html>";
        }
        _http_status = 200;
        return ft_http_status(getHttpStatus());
    }
    if (cgi_return == 1 )
    {
        _http_status = 508;
        return ft_http_status(getHttpStatus());
    }
    if (cgi_return == 3) {
        finished = false;
        _fd_file = cgi.get_fd_val();
    }
    _response_body_as_string = cgi.getRespBuffer();
    _http_status = 200;
    return ft_http_status(getHttpStatus());
}

int Request::read_body_request()
{
    std::ofstream in_file("temp_file", std::ios_base::app);

    if (_header.find("Content-Length") != _header.end() && _header["Content-Length"] != "")
    {
        char buffer_chr[BUFFER_SIZE];
        memset(buffer_chr, 0, BUFFER_SIZE);
        int content_length_read = recv(_read_fd, buffer_chr, BUFFER_SIZE, 0);
        in_file.write(buffer_chr, content_length_read);
        _content_actual_size -= content_length_read;
        if (_content_actual_size > 0)
            read_again = true;
        else
            read_again = false;
    }
    else if (_header.find("Transfer-Encoding") != _header.end())
    {
        post_transfer_encoding();
    }
    in_file.close();
    return read_again;
}

int Request::get_cgi_helper() const {
    return _check_cgi;
}