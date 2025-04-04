/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:34:26 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/23 13:31:28 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	c %= 256;
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (0);
}
/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	str[40] = "computer science";
	char	*ptr;
	char	*ptr2;
	int		ch = 'v';

	ptr = strchr(str, ch);
	ptr2 = ft_strchr(str, ch);
	if (ptr == ptr2)
		printf("OK");
	else
		printf("KO\ntrue = %s\nMine = %s\n", ptr, ptr2);
	return (0);
}*/
