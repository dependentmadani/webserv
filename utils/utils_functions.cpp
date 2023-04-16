/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_functions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:54:35 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/31 16:54:35 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void	*ft_memset(void *b, int c, int len)
{
	int		i;
	char	*s;

	s = (char *)b;
	i = 0;
	while (i < len)
		s[i++] = (char)c;
	return (b);
}

void	*ft_calloc(int count, int size)
{
	void	*ptr;

	ptr = (void *)malloc(count * size);
	if (!ptr)
		return (NULL);
	ptr = ft_memset(ptr, 0, count * size);
	return (ptr);
}

char	*str_cut(const char *s, int begin, int fin)
{
	int		i;
	char	*sp;

	i = 0;
	sp = (char *)ft_calloc(sizeof(char), (fin - begin + 1));
	while (begin < fin)
		sp[i++] = s[begin++];
	sp[i] = '\0';
	return (sp);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
