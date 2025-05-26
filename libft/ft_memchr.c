/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:07:12 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/26 12:00:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;
	unsigned char	*find;

	p = (unsigned char *)s;
	find = 0;
	while (s && (n--))
	{
		if (*p == (unsigned char)c)
		{
			find = p;
			break ;
		}
		p++;
	}
	return (find);
}

/*#include <stdio.h>

int	main(void)
{
    char	str[] = "ceci est un test";
    char	*p;
    char	*t;

    p = ft_memchr(str, 'y', 15);
    t = memchr(str, 'y', 15);
    if (p == t)
        printf("OK");
    else
        printf("KO\nyours = %s\ntest = %s\n", p, t);
    return (0);
}*/
