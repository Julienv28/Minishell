/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:41:59 by juvitry           #+#    #+#             */
/*   Updated: 2025/01/14 16:01:19 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_tablen(char const *s, char c)
{
	int	sep;
	int	tabs;
	int	i;

	i = 0;
	tabs = 0;
	sep = 1;
	while (s[i])
	{
		if (sep && s[i] != c)
		{
			sep = 0;
			tabs++;
		}
		if (s[i] == c)
			sep = 1;
		i++;
	}
	return (tabs);
}

static int	safe_malloc(char **tab, int position, int size)
{
	int	i;

	i = 0;
	tab[position] = malloc(sizeof(char) * (size + 1));
	if (!tab)
	{
		while (tab[i])
		{
			free (tab[i]);
			i++;
		}
		free(tab);
		return (1);
	}
	return (0);
}

static int	ft_tabset(char **tab, char const *s, char c)
{
	int	i;
	int	j;
	int	a;
	int	b;

	i = 0;
	j = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		a = i;
		while (s[i] && s[i] != c)
			i++;
		b = i;
		if (safe_malloc(tab, j, b - a))
			return (0);
		ft_strlcpy(tab[j], s + a, b - a + 1);
		j++;
		while (s[i] && s[i] == c)
			i++;
	}
	tab[j] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	tab = malloc(sizeof(char *) * (ft_tablen(s, c) + 1));
	if (!tab)
	{
		free (tab);
		return (NULL);
	}
	if (!ft_tabset(tab, s, c))
		return (NULL);
	return (tab);
}

/*#include <stdio.h>

int	main(int ac, char **av)
{
	char	**res;
	int		i;

	if (ac != 3)
	{
		printf ("Nombre d'arguments incorrects\n");
		return (0);
	}
	res = ft_split(av[1], av[2][0]);
	if (!res)
	{
		printf("**SYSTEM FAILURE**\n");
		return (0);
	}
	i = 0;
	while (res[i])
	{
		printf("tab[%d] = %s\n", i, res[i]);
		i++;
	}
	return (0);
}*/