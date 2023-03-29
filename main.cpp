/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:40:02 by sriyani           #+#    #+#             */
/*   Updated: 2023/03/29 17:29:29 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

#include "./parse/parsing.hpp"

int main(int ac, char **av)
{
    std::ifstream file(av[1]);
    parsing vars;
    parsing *pars = new parsing;
    if (ac == 2)
    {
        if (file.is_open())
        {
           vars.copy_file(pars, av[1]);
           vars.check_key(pars);
            std::cout << " File exist"<<std::endl;
        }
        else
          std::cout << " File doesn't exist"<<std::endl;
         
    }
    else
        std::cout<<"Error from number of arguments "<<std::endl;
           
}