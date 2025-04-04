/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:49:10 by juvitry           #+#    #+#             */
/*   Updated: 2025/01/14 16:01:18 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_starttrim(char const *s1, char const *set)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (set[i])
// 	{
// 		if (set[i] == s1[j])
// 			j++;
// 		i++;
// 	}
// 	return (j);
// }

// int	ft_endtrim(char const *s1, char const *set)
// {
// 	int	i;
// 	int	j;

// 	i = ft_strlen(s1) - 1;
// 	j = ft_strlen(set) - 1;
// 	while (j >= 0)
// 	{
// 		if (set[j] == s1[i])
// 			i--;
// 		j--;
// 	}
// 	return (i);
// }

static int	is_charset(char c, char const *set)
{
	while (*set)
	{
		if (c == *set++)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*result;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup((char *)s1));
	start = 0;
	end = ft_strlen(s1);
	while (is_charset(s1[start], set))
		start++;
	if (start == end)
	{
		result = ft_strdup("");
		if (!result)
			return (NULL);
		return (result);
	}
	while (is_charset(s1[end -1], set))
		end--;
	result = ft_substr(s1, start, end - start);
	return (result);
}

/*#include <stdio.h>

int	main(int ac, char **av)
{
	char	*result;

	if (ac != 3)
		return (0);
	result = ft_strtrim(av[1], av[2]);
	if (!result)
	{
		free (result);
		return (0);
	}
	printf("Avant ft_strtrim >> s1 = %s || set = %s\n", av[1], av[2]);
	printf("Apres ft_strtrim >> Resultat = %s \n", result);
	return (0);
}*/
