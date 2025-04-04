/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:05:40 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/23 14:55:56 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result;
	size_t	i;

	result = ft_strdup((char *)s);
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	return (result);
}

/*#include <stdio.h>

char    ft_test(unsigned int nbr, char c)
{
    printf("c'est good ! %d et %c\n", nbr, c);
    return (c - 32);
}

int main(void)
{
    char str[] = "bonjour";
    printf("res = %s\n", str);
    char *res = ft_strmapi(str, ft_test);
    printf("res = %s\n", res);
    return (0);
}*/
