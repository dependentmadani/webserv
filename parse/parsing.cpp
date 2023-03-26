/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 15:31:25 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/26 15:58:06 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

parsing::parsing(void)
{
    std::cout<<"constructor"<<std::endl;
}


parsing::~parsing()
{
    std::cout<<"destructor"<<std::endl;
}

void parsing::copy_file(std::string ptr)
{
    std::string line;
    std::ifstream file(ptr);
    
    int i = 0;
    while (getline(file, line))
        this->vec.push_back(line);    
}
void parsing::check_key()
{
    for(int i=0;i < this->vec.size() ;i++)
    {
        if (!this->vec[i].compare("server {"))
        {
            std::cout << "=----------BARCCCCCCCCCA----------="<<std::endl;
        }
        else
        {
            std::cout << "ERROR"<<std::endl;
            // exit(1);
        }
    }
}

std::vector<std::string>  parsing::get_vec()
{
    return this->vec;
}