/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/06/20 16:53:50 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include <string.h>
#include <sstream>

int ft_len(std::string s, char c)
{
    int len;
    int i = 0;

    len = 0;
    while (s[i])
    {
        if ((s[i] != c && len == 0) || (s[i] != c && (s[i - 1]) == c))
            len++;
        i++;
    }
    return (len);
}
int check_semi_colonne(std::string str)
{
    int count = std::count(str.begin(), str.end(), ';');
    size_t find = str.find(";");
    if (count != 1 && str[find - 1] != ';')
    {
        std::cout << " Error from semi colonne" << std::endl;
        exit(1);
    }
    return 0;
}

std::string parsing::trim(const std::string &str)
{
    std::size_t first = 0;
    std::size_t last = 0;
    if (str.size())
    {
        first = str.find_first_not_of(" \t\n\r\f\v");
        last = str.find_last_not_of(" \t\n\r\f\v");
    }

    return str.substr(first, last - first + 1);
}

bool whitespace(unsigned char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')
        return true;
    else
        return false;
}
void parsing::copy_file(parsing *pars, std::string ptr)
{
    std::string line;
    std::ifstream file(ptr);

    while (getline(file, line))
        pars->vec.push_back(line);
}

bool isNumber(char const *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
}

bool isWhitespace(std::string str)
{
    size_t len = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if ((str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\f' || str[i] == '\v' || str[i] == ' '))
            len++;
    }
    if (str.length() == len)
        return true;
    return false;
}

void print_error()
{
    std::cout << "Error missing rules" << std::endl;
    exit(1);
}

void parsing::check_listen(t_server *serv, std::string str)
{
    char *ptr;
    char *ss;
    std::string s2;
    str = trim(str);
    check_semi_colonne(str);
    if (!strncmp(str.c_str(), "listen", strlen("listen")))
    {
        if (!isblank(str[strlen("listen")]))
        {
            std::cout << "Error from listen" << std::endl;
            exit(1);
        }
        str = str.c_str() + strlen("listen");
        if (!str.length())
            print_error();
        str = trim(str);
        ss = const_cast<char *>(str.c_str());
        size_t ff = str.find(";");
        str = str.substr(0, ff);
        ptr = strtok(ss, ":");
        while (ptr != NULL)
        {
            if (isNumber(ptr))
                serv->ind_port = atoi(ptr);
            else
                serv->host = ptr;
            ptr = strtok(NULL, " ");
        }
    }
}

void parsing::check_server_name(t_server *serv, std::string str)
{
    std::string s2;
    str = trim(str);
    check_semi_colonne(str);
    if (!strncmp(str.c_str(), "server_name", strlen("server_name")))
    {
        if (!isblank(str[strlen("server_name")]))
        {
            std::cout << "Error from server_name" << std::endl;
            exit(1);
        }
        str = str.c_str() + strlen("server_name");
        str = trim(str);
        size_t ff = str.find(";");
        str = str.substr(0, ff);
        serv->server_name = str;
    }
}

void parsing::check_error_pages(t_server *serv, std::string str)
{
    str = trim(str);
    int flag = 0;
    std::vector<std::string> hold_error;
    check_semi_colonne(str);
    if (!strncmp(str.c_str(), "error_page", strlen("error_page")))
    {
        std::stringstream ss(str);
        std::string token;
        while (getline(ss, token, '\t'))
        {
            std::stringstream jo(token);
            while (getline(jo, token, ' '))
            {
                if (token != "\0")
                {
                    hold_error.push_back(token);
                }
            }
        }
        for (size_t i = 0; i < hold_error.size(); i++)
        {
            if (isNumber(hold_error[i].c_str()))
                serv->error_num.push_back(atoi(hold_error[i].c_str()));
            else if (hold_error[i].compare("error_page"))
            {
                size_t ff = hold_error[i].find(";");
                hold_error[i] = hold_error[i].substr(0, ff);
                if (ff != std::string::npos)
                    flag++;
                serv->error_page.push_back(hold_error[i]);
            }
        }
        if (hold_error.size() != 3 && (!(hold_error.size() == 4 && hold_error[hold_error.size() - 1] == "")))
            print_error();
    }
}

void parsing::check_max_client(t_server *serv, std::string str)
{
    char *ss;
    str = trim(str);
    check_semi_colonne(str);
    if (!strncmp(str.c_str(), "max_client_body_size", strlen("max_client_body_size")))
    {
        if (!isblank(str[strlen("max_client_body_size")]))
        {
            std::cout << "Error max_client_body_size" << std::endl;
            exit(1);
        }
        str = str.c_str() + strlen("max_client_body_size") + 1;
        str = trim(str);
        size_t ff = str.find(";");
        str = str.substr(0, ff);
        str = trim(str);
        if (!str.length())
            print_error();
        ss = const_cast<char *>(str.c_str());
        if (isNumber(ss))
            serv->max_client = atol(ss);
        else
        {
            std::cout << "Error max_client_body_size" << std::endl;
            exit(1);
        }
    }
}

void fill_methods(location *loc, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
    {
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
        {
            if (token != "\0")
                loc->methods.push_back(token);
        }
    }
}

void fill_index(location *loc, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
    {
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
        {
            if (token != "\0")
                loc->index.push_back(token);
        }
    }
}

void fill_cgi(location *loc, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
    {
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
        {
            if (token != "\0")
                loc->cgi_pass.push_back(token);
        }
    }
}

void fill_return(location *loc, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
    {
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
        {
            if (token != "\0")
            {
                if (atoi(token.c_str()))
                    loc->num_return = atoi(token.c_str());
                else
                    loc->return_url = token;
            }
        }
    }
}

void parsing::check_location(location *loc)
{
    char *ss;
    size_t j = 0;
    size_t flag = 2;
    std::string ptr;
    loc->location_flag = 0;
    loc->flag_auto = 0;
    for (size_t i = 0; i < loc->location.size(); i++)
    {

        if (!isWhitespace(loc->location[i]))
            ptr = trim(loc->location[i]);
        if (!strncmp(ptr.c_str(), "location", strlen("location")))
        {
            j = 0;
            ss = new char[ptr.size() - (strlen("location") - 1)];
            for (size_t i = strlen("location"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            loc->url_location = static_cast<std::string>(ss);
            loc->url_location = trim(loc->url_location);
            size_t found = loc->url_location.find("/");
            if (found == std::string::npos)
            {
                std::cout << "Error from location url" << std::endl;
                exit(1);
            }
            delete[] ss;
            flag++;
        }
        if (!strncmp(ptr.c_str(), "autoindex", strlen("autoindex")))
        {
            j = 0;
            loc->flag_auto++;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("autoindex") - 1)];
            for (size_t i = strlen("autoindex"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            if (!ptr.size())
            {
                std::cout << "Error from autoindex" << std::endl;
                exit(1);
            }
            delete[] ss;
            flag++;
            if (!strcmp(ptr.c_str(), "on"))
                loc->auto_index = true;
            else if (!strcmp(ptr.c_str(), "off"))
                loc->auto_index = false;
            else
            {
                std::cout << "Error from autoindex" << std::endl;
                exit(1);
            }
        }
        if (!strncmp(ptr.c_str(), "return", strlen("return")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("return") - 1)];
            for (size_t i = strlen("return"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            fill_return(loc, ptr);
            delete[] ss;
            flag++;
        }

        if (!strncmp(ptr.c_str(), "allow_methods", strlen("allow_methods")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("allow_methods") - 1)];
            for (size_t i = strlen("allow_methods"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            fill_methods(loc, ptr);
            for (size_t i = 0; i < loc->methods.size(); i++)
            {
                if (loc->methods.size() > 3 || (strcmp(loc->methods[i].c_str(), "POST") &&
                                                strcmp(loc->methods[i].c_str(), "GET") && strcmp(loc->methods[i].c_str(), "DELETE")))
                    std::cout << "Error from methodes" << std::endl;
            }
            delete[] ss;
            flag++;
        }
        if (!strncmp(ptr.c_str(), "root", strlen("root")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("root") - 1)];
            for (size_t i = strlen("root"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            if (!ptr.size())
            {
                std::cout << "Error from root" << std::endl;
                exit(1);
            }
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            loc->root_location = ptr;
            delete[] ss;
            flag++;
        }
        if (!strncmp(ptr.c_str(), "index", strlen("index")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("index") - 1)];
            for (size_t i = strlen("index"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            if (!ptr.size())
            {
                std::cout << "Error from index" << std::endl;
                exit(1);
            }
            fill_index(loc, ptr);
            delete[] ss;
            flag++;
        }
        if (!strncmp(ptr.c_str(), "cgi_pass", strlen("cgi_pass")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("cgi_pass") - 1)];
            for (size_t i = strlen("cgi_pass"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            if (!ptr.size())
            {
                std::cout << "Error from cgi_pass" << std::endl;
                exit(1);
            }
            fill_cgi(loc, ptr);
            if (loc->cgi_pass.size() % 2 != 0)
            {
                std::cout << "Error from cgi_pass" << std::endl;
                exit(1);
            }
            delete[] ss;
            flag++;
        }
        if (!strncmp(ptr.c_str(), "uploads", strlen("uploads")))
        {
            j = 0;
            check_semi_colonne(ptr);
            ss = new char[ptr.size() - (strlen("uploads") - 1)];
            for (size_t i = strlen("uploads"); i < ptr.size(); i++)
                ss[j++] = ptr[i];
            ss[j] = '\0';
            ptr = static_cast<std::string>(ss);
            ptr = trim(ptr);
            size_t ff = ptr.find(";");
            ptr = ptr.substr(0, ff);
            ptr = trim(ptr);
            if (!ptr.size())
            {
                std::cout << "Error from uploads" << std::endl;
                exit(1);
            }
            loc->uploads = ptr;
            delete[] ss;
            flag++;
        }
        if (isWhitespace(loc->location[i]))
        {
            flag++;
            loc->location_flag++;
        }
    }
    if (flag != loc->location.size())
    {
        std::cout << flag << " |ERROR FROM LOCATION |" << loc->location.size() << std::endl;
        exit(0);
    }
    if (loc->root_location.empty() || loc->methods.empty()) // ||  loc->cgi_pass.empty()|| !loc->flag_auto)
        print_error();
}

void parsing::check_server(s_parsing *pars, size_t len)
{
    for (size_t i = 0; i < len; i++)
        pars->serv[i]->loc = new location *[pars->serv[i]->server.size()];
    for (size_t i = 0; i < len; i++)
    {
        size_t num = 0;
        size_t flag = 2;
        pars->serv[i]->server_flag = 0;
        pars->serv[i]->ind_port = -1;
        pars->serv[i]->flag_max_client = 0;
        pars->serv[i]->location_falg = 0;
        pars->serv[i]->error_flag = 0;
        for (size_t j = 0; j < pars->serv[i]->server.size(); j++)
        {
            size_t found;
            if (isWhitespace(pars->serv[i]->server[j]))
            {
                pars->pars_flag++;
                flag++;
            }
            found = pars->serv[i]->server[j].find("listen");
            if (found != std::string::npos)
            {
                check_listen(pars->serv[i], pars->serv[i]->server[j]);
                flag++;
            }
            found = pars->serv[i]->server[j].find("server_name");
            if (found != std::string::npos)
            {
                check_server_name(pars->serv[i], pars->serv[i]->server[j]);
                flag++;
            }
            found = pars->serv[i]->server[j].find("max_client_body_size");
            if (found != std::string::npos)
            {
                pars->serv[i]->flag_max_client++;
                check_max_client(pars->serv[i], pars->serv[i]->server[j]);
                flag++;
            }
            found = pars->serv[i]->server[j].find("error_page");
            if (found != std::string::npos)
            {
                pars->serv[i]->error_flag++;
                check_error_pages(pars->serv[i], pars->serv[i]->server[j]);
                flag++;
            }
            found = pars->serv[i]->server[j].find("location");
            if (found != std::string::npos)
            {
                pars->serv[i]->location_falg++;
                pars->serv[i]->loc[num] = new location();
                for (size_t k = j; k < pars->serv[i]->server.size(); k++)
                {
                    size_t trouv = pars->serv[i]->server[k].find("}");
                    if (trouv != std::string::npos)
                    {
                        pars->serv[i]->loc[num]->location.push_back(pars->serv[i]->server[k]);
                        break;
                    }
                    else
                        pars->serv[i]->loc[num]->location.push_back(pars->serv[i]->server[k]);
                    flag++;
                }
                check_location(pars->serv[i]->loc[num]);
                pars->serv[i]->server_flag += pars->serv[i]->loc[num]->location_flag;
                pars->count_flag += pars->serv[i]->loc[num]->location_flag;
                pars->serv[i]->num_location = ++num;
            }
            pars->serv[i]->server[j].erase(std::remove_if(pars->serv[i]->server[j].begin(), pars->serv[i]->server[j].end(), whitespace), pars->serv[i]->server[j].end());
            if (!strncmp(pars->serv[i]->server[j].c_str(), "}", 1) && j == pars->serv[i]->server.size() - 1)
            {
                flag++;
                break;
            }
        }
        pars->pars_flag += pars->serv[i]->server_flag;
        if (((flag + num - 1) - pars->serv[i]->server_flag) != pars->serv[i]->server.size())
        {
            std::cout << "ERROR FROM SERVER " << std::endl;
            exit(0);
        }
        if (pars->serv[i]->ind_port < 0 || pars->serv[i]->host.empty() ||
            !pars->serv[i]->flag_max_client || !pars->serv[i]->location_falg ||
            !pars->serv[i]->error_flag || !pars->serv[i]->error_num.size() || !pars->serv[i]->error_page.size())
            print_error();
    }
}
void parsing::check_port(s_parsing *pars)
{
    std::vector<int> vec;
    for (int i = 0; i < pars->num_serv; i++)
        vec.push_back(pars->serv[i]->ind_port);
    for (int i = 0; i < pars->num_serv; i++)
    {
        for (int j = 0; j < pars->num_serv; j++)
        {
            if (vec[i] == vec[j] && i != j)
            {
                std::cout << vec[i] << "Error From port " << std::endl;
                exit(1);
            }
        }
    }
}
void parsing::check_key(s_parsing *pars)
{
    int k = 0;
    pars->serv = new t_server *[pars->vec.size()];
    pars->num_serv = 0;
    int flag = 0;
    int len = 0;
    size_t i = 0;
    pars->pars_flag = 0;
    pars->count_flag = 0;
    for (; i < pars->vec.size(); i++)
    {

        pars->vec[i].erase(std::remove_if(pars->vec[i].begin(), pars->vec[i].end(), whitespace), pars->vec[i].end());
        if (isWhitespace(pars->vec[i]))
            flag++;
        else if (!strcmp(pars->vec[i].c_str(), "server"))
        {
            pars->serv[k] = new t_server();
            for (size_t j = i + 1; j < pars->vec.size(); j++)
            {
                flag++;
                if (!strncmp(pars->vec[j].c_str(), "}", 1))
                {
                    pars->serv[k]->server.push_back(pars->vec[j]);
                    break;
                }
                else
                    pars->serv[k]->server.push_back(pars->vec[j]);
            }
            k++;
        }
    }
    pars->num_serv = k;
    check_server(pars, k);
    len = k - pars->pars_flag + pars->count_flag;
    check_port(pars);
    if ((flag + len) != (int)i)
    {
        std::cout << "ERROR FROM PARS " << std::endl;
        exit(0);
    }
}