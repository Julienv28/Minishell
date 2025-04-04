/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:23:35 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/24 16:15:41 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < n - 1 && s1[i] == s2[i])
		i++;
	return ((unsigned char )s1[i] - (unsigned char )s2[i]);
}

// #include <string.h>
// #include <stdio.h>

// int	main(int ac, char **av)
// {
// 	int	a;
// 	int	b;

// 	if (ac < 3)
// 	{
// 		printf("Il faut des elements a comparer !\n");
// 		return (0);
// 	}
// 	a = strncmp(av[1], av[2], 6);
// 	b = ft_strncmp(av[1], av[2], 6);
// 	if (a == b)
// 		printf("OK");
// 	else
// 		printf("KO\nTrue = %d\nYours = %d\n", a, b);
// 	return (0);
// }
