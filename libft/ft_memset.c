/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:34:34 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/09 19:37:37 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n--)
	{
		*p++ = c;
	}
	return (s);
}
/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	str[35] = "J'apprends a coder comme un grand.";
	char	str2[35] = "J'apprends a coder comme un grand.";

	printf("\nAvant memset(): %s\n \n", str);
	memset(str + 4, '*', 5);
	printf("Apres memset(): %s \n \n", str);
	printf("Reset: %s \n \n", str2);
	ft_memset(str2 + 4, '*', 5);
	printf("Apres ft_memset(): %s \n \n", str2);
	return (0);
}*/
