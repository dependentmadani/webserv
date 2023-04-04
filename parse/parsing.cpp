/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:37:13 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/04 17:29:36 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parsing.hpp"
#include <string.h>
#include <sstream>  


int ft_len(std::string s, char c)
{
    int len;
    int i =0;

    len = 0;
    while (s[i])
    {
        if ((s[i] != c && len == 0) || (s[i] != c && (s[i-1] ) == c))
            len++;
        i++;
    }
    return (len);
}

std::string parsing::trim(const std::string& str)
{
    std::size_t first = str.find_first_not_of(" \t\n\r\f\v");
    std::size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
}

bool whitespace(unsigned char c) 
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
        return true;
    } else {
        return false;
    }
}
void parsing::copy_file(parsing *pars, std::string ptr)
{
    std::string line;
    std::ifstream file(ptr);
    
    int i = 0;
    while (getline(file, line))
        pars->vec.push_back(line);
     
}

bool isNumber(char * str)
{
    for (int i =0;str[i];i++) 
    {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
}

bool isWhitespace(std::string str)
{
    int len = 0;
    for (int i = 0; i < str.length(); i++)
    {
          if ((str[i] == '\t' || str[i] == '\n' || str[i] == '\r' ||  str[i] == '\f' || str[i] == '\v' || str[i] == ' '))
            len++;
    }
    if (str.length() == len)
        return  true;
    return false;
}

void parsing::check_listen(t_server *serv, std::string str)
{
    char  *ptr;
    char *ss;
    std::string s2;
    str = trim(str);
    if (!strncmp(str.c_str(), "listen", strlen("listen")))
    {

            str = str.c_str() + strlen("listen");
            str = trim(str);
            ss = const_cast<char*>(str.c_str());
            ptr = strtok(ss, ":");
            while ( ptr != NULL)
            {
                if (isNumber(ptr))
                    serv->ind_port = atoi(ptr);
                else
                    serv->host = ptr;
                ptr = strtok(NULL,  " ");
            }
    }
}

void parsing::check_server_name(t_server *serv, std::string str)
{
    char  *ptr;
    char *ss;
    std::string s2;
    str = trim(str);
    if (!strncmp(str.c_str(), "server_name", strlen("server_name")))
    {
        str = str.c_str()+strlen("server_name");
        str = trim(str);
        serv->server_name = str;
    }
}


void parsing::check_error_pages(t_server *serv, std::string str)
{
    char *ss;
    char *s2;
    str = trim(str);
    int lent =  0;
    int j = 10;
    int k;
    if (!strncmp(str.c_str(), "error_page",strlen("error_page")))
    {
        lent = ft_len(str,' ');
        if (lent != 3)
            lent = ft_len(str, 9)+ lent -1;
        ss = const_cast<char*>(str.c_str());
        s2 = new char[strlen(ss) -j + 1];
        for (int i = 0; i < lent -1 ; i++)
        {
            for ( ; ss[j] == ' ' || ss[j]  == '\t' ; j++);
            k = 0;
            for (; ss[j] ; j++)
            {
                if (ss[j] == ' ' || ss[j] ==  '\t')
                    break;
                s2[k++] = ss[j];
            }
            s2[k] = '\0';
            std::string ptr(s2);
            if (isNumber(s2))
                 serv->error_num.push_back(atoi(s2));
            else
                serv->error_page.push_back(ptr);
        }
        delete [] s2;  
    }
}

void parsing::check_max_client(t_server *serv, std::string str)
{
    char *ss;
    str = trim(str);
    if (!strncmp(str.c_str(), "max_client_body_size", strlen("max_client_body_size")))
    {
            str = str.c_str()+ strlen("max_client_body_size")+1;
            str = trim(str);
            ss = const_cast<char*>(str.c_str());
            if (isNumber(ss))
                serv-> max_client = atol(ss);
             else
                std::cout<<"Error max_client_body_size"<<std::endl;
    }
}

// void fill_methods(std::string str)
// {
// 	std::stringstream ss(str);
// 	while(getline(ss,str,(char)9))
// 	{
// 		// while(getline(ss,str, ' '))
// 		// {
// 		// 	// std::cout << str << std::endl; 
// 		// }
// 	}
// 	// std::cout << "||||||||"<< str << std::endl; 
// 	std::stringstream jo(str);
// 	while(getline(jo,str, ' '))
// 	{
// 		std::cout << str << std::endl;
// 	}
// 	// std::cout <<"----------" <<str << std::endl; 
	
// }

void fill_methods(t_server *serv, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
	{
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
		{
			if (token != "\0")
				serv->loc->methods.push_back(token);
            // std::cout << token << std::endl;
        }
    }
}

void fill_index(t_server *serv, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
	{
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
		{
			if (token != "\0")
				serv->loc->index.push_back(token);
            // std::cout << token << std::endl;
        }
    }
}

void fill_cgi(t_server *serv, std::string str)
{
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, '\t'))
	{
        std::stringstream jo(token);
        while (getline(jo, token, ' '))
		{
			if (token != "\0")
				serv->loc->cgi_pass.push_back(token);
            // std::cout << token << std::endl;
        }
    }
}


void parsing::check_location(t_server *serv, std::string str)
{
    char *ss;
    int  j = 0;
	 std::string ptr;
    // str = trim(str);
	// serv->loc->index = true;
	for (int i = 0; i < serv->loc->location.size() ; i++)
    {
		std::string ptr = trim(serv->loc->location[i]);
		
		if (!strncmp(ptr.c_str(), "location", strlen("location")))
    	{
            j = 0;
			ss = new char [ptr.size() - (strlen("location") - 1)];
        	for (int i = strlen("location"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
        	serv->loc->url_locaton = static_cast<std::string>(ss);
			serv->loc->url_locaton = trim(serv->loc->url_locaton);
			size_t found= serv->loc->url_locaton.find("/");
			if(found == std::string::npos)
				std::cout<<"Error from location url"<<std::endl;
			delete [] ss;
    	}
		if (!strncmp(ptr.c_str(), "autoindex", strlen("autoindex")))
		{
            j = 0;
			ss = new char [ptr.size() - (strlen("autoindex") - 1)];
        	for (int i = strlen("autoindex"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
			ptr = static_cast<std::string>(ss);
			ptr = trim(ptr);
            delete [] ss;
			if (!strcmp(ptr.c_str(), "on"))
				serv->loc->auto_index = true;
			else
				serv->loc->auto_index = false;
		}
		if (!strncmp(ptr.c_str(), "allow_methods", strlen("allow_methods")))
		{
            j = 0;
			ss = new char [ptr.size() - (strlen("allow_methods") - 1)];
        	for (int i = strlen("allow_methods"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
			ptr = static_cast<std::string>(ss);
			ptr = trim(ptr);
			fill_methods(serv, ptr);
			for (size_t i = 0; i < serv->loc->methods.size(); i++)
			{
				if (serv->loc->methods.size() > 3 || (strcmp(serv->loc->methods[i].c_str() ,"POST") && 
					strcmp(serv->loc->methods[i].c_str() ,"GET") && strcmp(serv->loc->methods[i].c_str() ,"DELETE")))
					std::cout<< "Error from methodes" <<std::endl;
			}
            delete [] ss;
		}

        if (!strncmp(ptr.c_str(), "root", strlen("root")))
		{
            j = 0;
			ss = new char [ptr.size() - (strlen("root") - 1)];
        	for (int i = strlen("root"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
			ptr = static_cast<std::string>(ss);
			ptr = trim(ptr);
            serv->loc->root_locaton = ptr;
            delete [] ss;
		}
        if (!strncmp(ptr.c_str(), "index", strlen("index")))
		{
            j = 0;
			ss = new char [ptr.size() - (strlen("index") - 1)];
        	for (int i = strlen("index"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
			ptr = static_cast<std::string>(ss);
			ptr = trim(ptr);
            fill_index(serv, ptr);
            delete [] ss;
		}
        if (!strncmp(ptr.c_str(), "cgi_pass", strlen("cgi_pass")))
		{
            j =0;
			ss = new char [ptr.size() - (strlen("cgi_pass") - 1)];
        	for (int i = strlen("cgi_pass"); i < ptr.size() ; i++)
        	    ss[j++] = ptr[i];
        	ss[j] = '\0';
			ptr = static_cast<std::string>(ss);
			ptr = trim(ptr);
            fill_cgi(serv, ptr);
            delete [] ss;
    	}    
	}
    for (size_t i = 0; i < serv->loc->cgi_pass.size(); i++)
    {
        std::cout<<"-----------------|"<<serv->loc->cgi_pass[i]<<std::endl;
    }
    
}


void parsing::check_server(s_parsing *pars, int len)
{
	for (int i = 0; i < len ; i++)
    {
		int len = 0;
       for (int j = 0; j < pars->serv[i]->server.size(); j++)
       {
		len++;
        // pars->serv[i]->server[j].erase(std::remove_if(pars->serv[i]->server[j].begin(),pars->serv[i]->server[j].end(), whitespace), pars->serv[i]->server[j].end());
        if (!strncmp(pars->serv[i]->server[j].c_str(), "}", 1))
            break ;
       }
	   pars->serv[i]->lent_server.push_back(len);
    }
    for (int i = 0; i < len ; i++)
    {
       for (int j = 0; j < pars->serv[i]->server.size(); j++)
       {
        size_t found;
        found = pars->serv[i]->server[j].find("listen");
        if (found != std::string::npos)
        {
            check_listen(pars->serv[i], pars->serv[i]->server[j]);

        }
        found = pars->serv[i]->server[j].find("server_name");
        if (found != std::string::npos)
        {
            check_server_name(pars->serv[i], pars->serv[i]->server[j]);
        }
        found = pars->serv[i]->server[j].find("max_client_body_size");
        if (found != std::string::npos)
        {
            check_max_client(pars->serv[i], pars->serv[i]->server[j]);
        }
        found = pars->serv[i]->server[j].find("error_page");
        if (found != std::string::npos)
        {
            check_error_pages(pars->serv[i], pars->serv[i]->server[j]);
        }
        found = pars->serv[i]->server[j].find("location");
        if (found != std::string::npos)
        {
            pars->serv[i]->loc = new location(); 
			for (int k = j; k < pars->serv[i]->server.size() ; k++)
    		{
				size_t trouv = pars->serv[i]->server[k].find("]");
				if (trouv != std::string::npos)
					break ;
				else
					pars->serv[i]->loc->location.push_back(pars->serv[i]->server[k]);
        	}
			check_location(pars->serv[i], pars->serv[i]->server[j]);
			
        }
        
        pars->serv[i]->server[j].erase(std::remove_if(pars->serv[i]->server[j].begin(),pars->serv[i]->server[j].end(), whitespace), pars->serv[i]->server[j].end());
        if (!strncmp(pars->serv[i]->server[j].c_str(), "}", 1))
            break ;
        // else
        //     std::cout <<" ERRRRRROR "<<std::endl;
        // std::cout<< j << " ************ |"<<pars->serv[i]->server[j]<<"|  ************"<<std::endl;
        // if (pars->serv[i]->server[j])
       }
    }
	
    // check_server_name(pars);
    // check_error_pages(pars);
    // check_client_max_size(pars);
}

void  parsing::check_key(s_parsing *pars)
{
    int k = 0;
    pars->serv = new t_server*[pars->vec.size()];
    for(int i=0;i < pars->vec.size() ;i++)
    {
        pars->vec[i].erase(std::remove_if(pars->vec[i].begin(), pars->vec[i].end(), whitespace), pars->vec[i].end());
        if (!strcmp(pars->vec[i].c_str(), "server") || pars->vec[i] == "\0")
        {
            // std::cout<< i << " ************ |"<< pars->vec[i]<<"|  ************"<<std::endl;
            if(!strcmp(pars->vec[i].c_str(), "server"))
            {
                pars->serv[k] = new t_server();
                for(int j = i+1; j < pars->vec.size() ;j++)
                {
                    if (!strncmp(pars->vec[j].c_str(),"}",1))
                    {
                        pars->serv[k]->server.push_back(pars->vec[j]);
                        break ;
                    }
                    else
                        pars->serv[k]->server.push_back(pars->vec[j]);
                }
                k++;
            }
        }
        // else
        //     std::cout<< i << " Error "<<std::endl;
    }
   check_server(pars, k);
    
}

std::vector<std::string>  parsing::get_vec()
{
    return this->vec;
}