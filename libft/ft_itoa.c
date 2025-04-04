/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:02:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/01/14 16:01:20 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	nb_size(long int nb)
{
	int	i;

	i = 1;
	if (nb < 0)
	{
		i++;
		nb *= (-1);
	}
	while (nb >= 10)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

static char	*ft_charconv(long int nb, char *str, int *i)
{
	if (nb < 0)
	{
		str[*i] = '-';
		(*i)++;
		nb *= (-1);
	}
	if (nb >= 0 && nb <= 9)
	{
		str[*i] = nb + 48;
		(*i)++;
	}
	else if (nb > 9)
	{
		ft_charconv(nb / 10, str, i);
		ft_charconv(nb % 10, str, i);
	}
	if (nb >= 0 && nb <= 9)
		str[*i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	long int		a;
	int				i;
	char			*str;

	a = n;
	str = malloc(sizeof(char) * nb_size(a) + 1);
	if (!str)
		return (NULL);
	i = 0;
	ft_charconv(a, str, &i);
	return (str);
}
/*#include <stdio.h>

int	main(int ac, char **av)
{
	char *result;
	if (ac != 2)
		return (0);
	result = ft_itoa(atoi(av[1]));
	printf("%s\n", result);
	return (0);
}*/