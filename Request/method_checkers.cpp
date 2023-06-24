/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:45:58 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/06/17 11:46:09 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::get_request_resource()
{
    struct stat stat_buff;

    for (std::vector<std::string>::iterator b = _file_name_path.begin(); b != _file_name_path.end(); ++b)
    {
        int stat_return = stat((*b).c_str(), &stat_buff);
        if (stat_return != -1)
        {
            _available_file_path = *b;
            _content_length = stat_buff.st_size;
            return 0;
        }
    }
    return 1;
}

int Request::get_request_resource_for_delete()
{
    struct stat stat_buff;
    
    if (!_file_name_path.empty()) {
        for (std::vector<std::string>::iterator b = _file_name_path.begin(); b != _file_name_path.end(); ++b)
        {
            int stat_return = stat((*b).c_str(), &stat_buff);
            if (stat_return != -1)
            {
                _available_file_path = *b;
                _content_length = stat_buff.st_size;
                return 0;
            }
        }
    }
    else {
        int stat_return = stat(_directory_path.c_str(), &stat_buff);
        if (stat_return != -1)
        {
            _available_file_path = _directory_path;
            _content_length = stat_buff.st_size;
            return 0;
        }
    }
    return 1;
}

int Request::get_resource_type()
{
    return _file_directory_check;
}

