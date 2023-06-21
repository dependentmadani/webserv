/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_functions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:54:35 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/05/28 11:11:43 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void *ft_memset(void *b, int c, int len)
{
	int i;
	char *s;

	s = (char *)b;
	i = 0;
	while (i < len)
		s[i++] = (char)c;
	return (b);
}

void *ft_calloc(int count, int size)
{
	void *ptr;

	ptr = (void *)malloc(count * size);
	if (!ptr)
		return (NULL);
	ptr = ft_memset(ptr, 0, count * size);
	return (ptr);
}

char *str_cut(const char *s, int begin, int fin)
{
	int i;
	char *sp;

	i = 0;
	sp = (char *)ft_calloc(sizeof(char), (fin - begin + 1));
	while (begin < fin)
		sp[i++] = s[begin++];
	sp[i] = '\0';
	return (sp);
}

size_t ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

std::string randomstring(int length)
{
	std::string str;
	srand(time(0));
	for (int i = 0; i < length; ++i)
	{
		char c = 'a' + (rand() % 26);
		str += c;
	}

	return str;
}

std::string readFileToString(const std::string &filename)
{
	std::ifstream file(filename);
	std::string content((std::istreambuf_iterator<char>(file)),
						(std::istreambuf_iterator<char>()));
	return content;
}
std::string trim(const std::string &str)
{
	std::regex pattern("^\\s+|\\s+$");

	return std::regex_replace(str, pattern, "");
}

void deleteCharArray(char **arr, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		delete[] arr[i];
		delete[] arr;
	}
}

void free_doublep(char **str)
{
	int j = 0;
	while (str[j] != NULL)
		j++;
	ft_free(str, j);
}