/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:49:20 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/23 14:42:25 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!big || !little)
		return (0);
	i = 0;
	if (!little || !little[i])
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && big[i + j]
			&& little[j] && i + j < len)
			j++;
		if (little[j] == '\0')
			return ((char *)big + i);
		i++;
	}
	return (0);
}

/*#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>

int 	main(int ac, char **av)
{
	char *result;
	char *compare;
	
	if (ac != 4)
	{
		printf("Nombre de parametres incorrect\n");
		return (0);
	}
	result = ft_strnstr(av[1], av[2], atoi(av[3]));
	compare = strnstr(av[1], av[2], atoi(av[3]));
	if (result == compare)
		printf("OK");
	else 
		printf("KO\nMa fonction : %s\nOriginale : %s\n", result, compare);
	return (0);
}*/
// gcc ft_strnstr.c `pkg-config --libs libbsd` = pour compiler.