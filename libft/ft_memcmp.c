/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:55:00 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/24 15:38:23 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;

	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	if (!n)
		return (0);
	while (--n && *a == *b)
	{
		a++;
		b++;
	}
	if (*a - *b > 0)
		return (5);
	if (*a - *b < 0)
		return (-1);
	return (0);
}

// #include <stdio.h>

// int	main(int ac, char **av)
// {
// 	int	cmp;
// 	int	test;

// 	if (ac < 2)
// 		return (0);
// 	cmp = ft_memcmp(av[1], av[2], 5);
// 	test = memcmp(av[1], av[2], 5);
// 	if (cmp != 0)
// 		printf("Les 2 chaines different\n");
// 	else
// 		printf("Les 2 chaines sont peut-etre similaires \n");
// 	if (cmp == test)
// 		printf("OK \n");
// 	else
// 		printf("KO\nTest = %d\nValeur = %d", test, cmp);
// 	return (0);
// }
