/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:58:13 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/26 12:09:42 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_totlen(const char *s1, const char *s2)
{
	int	a;
	int	b;
	int	c;

	a = ft_strlen(s1);
	b = ft_strlen(s2);
	c = a + b;
	return (c);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*result;

	result = malloc(sizeof(char) * ft_totlen(s1, s2) + 1);
	if (!result)
		return (NULL);
	*result = 0;
	ft_strcat(result, (char *)s1);
	ft_strcat(result, (char *)s2);
	return (result);
}

/*#include <stdio.h>

int	main(int ac, char **av)
{
    char	*final;

    if (ac < 3)
        return (0);
    final = ft_strjoin(av[1], av[2]);
    if (final == NULL)
    {
        free(final);
        printf("ERROR");
        return (0);
    }
    printf("Final = %s \n", final);
    return (0);
}*/
