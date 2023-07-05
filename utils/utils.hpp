/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:52:59 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/28 11:11:06 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

void *ft_calloc(int count, int size);
char **ft_freee(char **arr, int j);
char **ft_split(char const *ss, char c);
char *str_cut(const char *s, int begin, int fin);
size_t ft_strlen(const char *s);
void *ft_memset(void *b, int c, int len);
std::string randomstring(int length);
std::string readFileToString(const std::string &filename);
std::string trim(const std::string &str);
void deleteCharArray(char **arr, size_t size);
void free_doublep(char **str);
void ft_free(char **arr, int j);
#endif
