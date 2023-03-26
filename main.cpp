/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/26 15:23:20 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

#include "./parse/parsing.hpp"

int main(int ac, char **av)
{
    std::ifstream file(av[1]);
    parsing pars;
    if (ac == 2)
    {
        if (file.is_open())
        {
            pars.copy_file(av[1]);
            pars.check_key();
            std::cout << " File exist"<<std::endl;
        }
        else
          std::cout << " File doesn't exist"<<std::endl;
         
    }
    else
        std::cout<<"Error from number of arguments "<<std::endl;
           
}