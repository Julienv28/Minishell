/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:43:58 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/23 14:23:15 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char )c)
			return ((char *)s + i);
		i--;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	str[6] = "teste";
// 	char	*ptr;
// 	char	*ptr2;
// 	int		ch = 1024;

// 	ptr = strrchr(str, ch);
// 	ptr2 = ft_strrchr(str, ch);
// 	if (ptr == ptr2)
// 		printf("OK");
// 	else
// 		printf("KO\ntrue = %s\nMine = %s\n", ptr, ptr2);
// 	return (0);
// }
