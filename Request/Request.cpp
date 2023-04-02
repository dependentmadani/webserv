/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:16:44 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/27 12:16:45 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _method(), _path(), _protocol() ,_header(), http_code()
{
    _http_status = 0;
}

Request::~Request()
{ }

int     Request::ParseRequest(char *request_message)
{
    char **splited_request = ft_split(request_message, '\n');

    this->FirstLinerRequest(splited_request[0]);
    this->HeaderRequest(request_message);
    if (check_method())
        return ft_http_status(_http_status);
    this->is_request_well_formed();
    return 0;
}

int    Request::FirstLinerRequest(char *request_message)
{
    _first_liner_header = std::string(request_message);
    // std::cout << "_first_liner value: " << _first_liner_header << std::endl;
    char **split_first_liner = ft_split(request_message, ' ');
    _method = std::string(split_first_liner[0]);
    if (!split_first_liner[1])
        return -1;
    _path = std::string(split_first_liner[1]);
    if (!split_first_liner[2])
        return -1;
    _protocol = std::string(split_first_liner[2]);
    return 0;
}

int    Request::HeaderRequest(char *request_message)
{
    // char **splited_header = ft_split(request_message, '\r');
    char *splited_header = strtok(request_message, "\r\n");

    splited_header = strtok(NULL, "\r\n");
    while (splited_header != NULL)
    {
        char **split_each_line = ft_split(splited_header, ':');
        _header[std::string(split_each_line[0])] = std::string(split_each_line[1]);
        splited_header = strtok(NULL, "\r\n");
    }
    // std::cout << "all good for now :) !" << std::endl;
    return 0;
}

int Request::is_request_well_formed()
{
    if (_method == "POST" && !is_available(std::string("Transfer-Encoding"), std::string("chunked")))
    {
        _http_status = 501;
        std::cout << "not good for now 1!" << std::endl;
        return ft_http_status(getHttpStatus());
    }
    if (_method == "POST" && !is_available(std::string("Transfer-Encoding"), std::string("")) && !is_available(std::string("Content-Length"), std::string("")))
    {
        _http_status = 400;
        std::cout << "not good for now 2!" << std::endl;
        return ft_http_status(getHttpStatus());
    }
    if (url_characters_checker())
    {
        _http_status = 400;
        std::cout << "not good for now 3!" << std::endl;
        return ft_http_status(getHttpStatus());
    }
    if (getPath().size() > 2048)
    {
        _http_status = 414;
        std::cout << "not good for now 4!" << std::endl;
        return ft_http_status(getHttpStatus());
    }
    // Need to check If request body is larger than client max body size set in config file.
    //need to know how I will receive the parsed config file
    std::cout << "all good for now :) !" << std::endl;
    return 0;
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
int Request::check_method()
{
    if (_method == "GET" || _method == "POST" || _method == "DELETE")
    {
        _http_status = 200;
        return 0;
    }
    if (_method == "OPTIONS" || _method == "HEAD" || _method == "PUT" || _method == "TRACE" || _method == "CONNECT")
    {
        _http_status = 501;
        return -1;
    }
    _http_status = 501;
    return -1;
}

//this function will be able to use response, where it will build a http message
int Request::ft_http_status(int value)
{
    (void)value;
    return 0;
}

void    Request::ft_mime_type()
{
    //text
    mime_type["html"]  = "text/html";
    mime_type["htm"]   = "text/html";
    mime_type["shtml"] = "text/html";
    mime_type["css"]   = "text/css";
    mime_type["xml"]   = "text/xml";
    //image
    mime_type["gif"]   = "image/gif";
    mime_type["jpeg"]  = "image/jpeg";
    mime_type["jpg"]   = "image/jpeg";
    //application
    mime_type["js"]    = "application/javascript";
    mime_type["atom"]  = "application/atom+xml";
    mime_type["rss"]   = "application/rss+xml";
    //text
    mime_type["mml"]   = "text/mathml";
    mime_type["txt"]   = "text/plain ";
    mime_type["jad"]   = "text/vnd.sun.j2me.app-descriptor";
    mime_type["wml"]   = "text/vnd.wap.wml";
    mime_type["htc"]   = "text/x-component";
    //image
    mime_type["avif"]  = "image/avif";
    mime_type["png"]   = "image/png";
    mime_type["svg"]   = "image/svg+xml";
    mime_type["svgz"]  = "image/svg+xml";
    mime_type["tif"]   = "image/tiff";
    mime_type["tiff"]  = "image/tiff";
    mime_type["wbmp"]  = "image/vnd.wap.wbmp";
    mime_type["webp"]  = "image/webp";
    mime_type["ico"]   = "image/x-icon";
    mime_type["jng"]   = "image/x-jng";
    mime_type["bmp"]   = "image/x-ms-bmp";
    //font
    mime_type["woff"]  = "font/woff";
    mime_type["woff2"] = "font/woff2";
    //application
    mime_type["jar"]   = "application/java-archive";
    mime_type["war"]   = "application/java-archive";
    mime_type["ear"]   = "application/java-archive";
    mime_type["json"]  = "application/json";
    mime_type["hqx"]   = "application/mac-binhex40";
    mime_type["doc"]   = "application/msword";
    mime_type["pdf"]   = "application/pdf";
    mime_type["ps"]    = "application/postscript";
    mime_type["eps"]   = "application/postscript";
    mime_type["ai"]    = "application/postscript";
    mime_type["rtf"]   = "application/rtf";
    mime_type["m3u8"]  = "application/vnd.apple.mpegurl";
    mime_type["kml"]   = "application/vnd.google-earth.kml+xml";
    mime_type["kmz"]   = "application/vnd.google-earth.kmz";
    mime_type["xls"]   = "application/vnd.ms-excel";
    mime_type["eot"]   = "application/vnd.ms-fontobject";
    mime_type["ppt"]   = "application/vnd.ms-powerpoint";
    mime_type["odg"]   = "application/vnd.oasis.opendocument.graphics";
    mime_type["odp"]   = "application/vnd.oasis.opendocument.presentation";
    mime_type["ods"]   = "application/vnd.oasis.opendocument.spreadsheet";
    mime_type["odt"]   = "application/vnd.oasis.opendocument.text";
    mime_type["pptx"]  = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime_type["xlsx"]  = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_type["docx"]  = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime_type["wmlc"]  = "application/vnd.wap.wmlc";
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

std::string Request::getPath() const
{
    return _path;
}

int     Request::getHttpStatus() const
{
    return _http_status;
}
