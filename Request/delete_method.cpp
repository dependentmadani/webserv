/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_method.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:40:29 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/06/17 11:41:37 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

int Request::DELETE_method()
{
    if (this->get_request_resource_for_delete())
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
        if (if_location_has_cgi())
        {
            if (is_dir_has_index_files())
            {
                // run cgi on requested file with DELETE REQUEST METHOD
                // and check if this directory has an index file and cgi
                // then return code depend on cgi
                if (!this->delete_all_folder_content(_directory_path + "/" + _parse->serv[_server_index]->loc[_location_index]->index[0], FILE))
                {
                    _http_status = 404;
                    return ft_http_status(getHttpStatus());
                }
                //// TODO: neeed to bee check
                _http_status = 204;
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
                _http_status = 204;
                return ft_http_status(getHttpStatus());
            }
            else
            {
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
        // the request contains a conflit because it does not have a / at the end
        _http_status = 409;
        return ft_http_status(getHttpStatus());
    }
    return 0;
}

int Request::Is_file_for_DELETE()
{
    if (this->if_location_has_cgi())
    {
        // nothing to do here for the moment. waiting for cgi to be done.
        // return code depend on cgi
        return this->request_run_cgi();
    }
    else
    {
        return this->delete_all_folder_content(_file_name_path[0], FILE);
    }
    return 0;
}

 /////////cheeck this function
int Request::delete_all_folder_content(std::string folder_file, int type)
{
    if (type == DIRECTORY)
    {
        // give all the files and directories available in this directory
        get_all_subdirectories(folder_file);
        std::vector<std::string>::iterator beg = _list_files_directories.begin();
        for (; beg != _list_files_directories.end(); ++beg) {
            if (access((*beg).c_str(), R_OK) != 0)
            {
                _directory_path = *beg;
                return 0;
            }
            if (!std::remove((*beg).c_str()))
            {
                std::cout << "well removed directory" << std::endl;
            }
        }
        if (!std::remove(folder_file.c_str()))
        {
            std::cout << "well removed directory" << std::endl;
        }
    }
    else if (type == FILE)
    {
        if (access(folder_file.c_str(), R_OK) != 0)
        {
            _directory_path = folder_file;
            return 0;
        }
        if (!std::remove(folder_file.c_str()))
        {
            std::cout << "well removed the file" << std::endl;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void Request::get_all_subdirectories(std::string folder_name) {
    DIR *dir;
    struct dirent *ent;
    struct stat stat_buff;


    while ((dir = opendir(folder_name.c_str())) == NULL) {
        return ;
    }
    while ((ent = readdir(dir)) != NULL)
    {
        stat(ent->d_name, &stat_buff);
        if (std::string(ent->d_name) == "." || std::string(ent->d_name) == "..") {
            continue;
        }
        if (S_ISDIR(stat_buff.st_mode))
        {
            get_all_subdirectories(std::string(folder_name + "/" + ent->d_name));
            _list_files_directories.push_back(folder_name + "/" +std::string(ent->d_name));
        }
        else if (S_ISREG(stat_buff.st_mode))
        {
            _list_files_directories.push_back(folder_name + "/" + std::string(ent->d_name));
        }
    }
    closedir(dir);
}

int Request::has_write_access_on_folder()
{
    int check_access = 0;

    check_access = access(_directory_path.c_str(), R_OK);
    if (check_access != 0)
        return 0;
    return 1;
}

