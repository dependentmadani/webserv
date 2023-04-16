/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:52:14 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/31 16:52:32 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

static int	string_count(const char *set, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (set[i])
	{
		if (set[i] != '\0' && set[i] != c)
		{
			count++;
			while (set[i] && set[i] != c)
				i++;
		}
		else if (set[i])
			i++;
	}
	return (count);
}

static char	**ft_freee(char **arr, int j)
{
	while (j > 0)
		free(arr[j--]);
	free(arr);
	return (NULL);
}

static char	**ft_split2_func(char **arr1, const char *str, char c)
{
	size_t		i;
	size_t		j;
	int			k;

	i = 0;
	j = 0;
	k = -1;
	while (i <= ft_strlen(str))
	{
		if (str[i] != c && k < 0)
			k = i;
		else if ((str[i] == c || i == ft_strlen(str)) && k >= 0)
		{
			arr1[j] = str_cut(str, k, i);
			if (!arr1[j])
				return (ft_freee(arr1, j));
			j++;
			k = -1;
		}
		i++;
	}
	arr1[j] = 0;
	return (arr1);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = (char **)ft_calloc((string_count(s, c) + 1), sizeof(char *));
	if (!arr)
		return (NULL);
	arr = ft_split2_func(arr, s, c);
	return (arr);
}
