/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:58:45 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/24 18:46:15 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;

	if (!s)
		return (NULL);
	if (len + start > ft_strlen(s))
		len = ft_strlen(s) - start;
	if (start >= ft_strlen(s) || len <= 0)
		len = 0;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		result[0] = '\0';
		return (result);
	}
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = 0;
	return (result);
}

// j = 0;
// 	while (s[j])
// 	{
// 		if (j >= start && i < len)
// 		{
// 			result[i] = s[j];
// 			i++;
// 		}
// 		j++;
// 	}
// #include <stdio.h>

// int	main(int ac, char **av)
// {
// 	char	*str;

// 	if (ac != 4)
// 	{
// 		printf("Nombre d'arguments incorrect \n");
// 		return (0);
// 	}
// 	str = ft_substr(av[1], atoi(av[2]), atoi(av[3]));
// 	if (!str)
// 	{
// 		printf("**AN ERROR OCCURRED**\n");
// 		return (0);
// 	}
// 	printf("Before ft_substr >> %s \n\n", av[1]);
// 	printf("After ft_substr >> %s \n", str);
// 	return (0);
// }
