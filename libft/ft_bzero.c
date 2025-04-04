/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:01:32 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/12 13:21:07 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <strings.h>

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n--)
		*p++ = '\0';
}

/*#include <stdio.h>

int	main(int ac, char **av)
{	
	if (ac < 3)
		return (0);
	bzero(av[1], 5);
	printf("Effet de bzero: %s \n \n", av[1]);
	ft_bzero(av[2], 5);
	printf("Effet de ft_bzero: %s \n \n", av[2]);
}*/
