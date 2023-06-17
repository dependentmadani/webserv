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
        // std::cout << "shouldnt be here aaaa hamid :)" << std::endl;
        // nothing to do here for the moment. waiting for cgi to be done.
        // return code depend on cgi
        return this->request_run_cgi();
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

