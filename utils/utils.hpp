/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbadaoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:52:59 by mbadaoui          #+#    #+#             */
/*   Updated: 2023/03/31 16:53:01 by mbadaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <stdlib.h>

void	       *ft_calloc(int count, int size);
char          **ft_split(char const *ss, char c);
char	       *str_cut(const char *s, int begin, int fin);
size_t          ft_strlen(const char *s);
void    *ft_memset(void *b, int c, int len);


#endif
