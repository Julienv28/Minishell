/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:56:48 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/23 10:57:16 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (8);
	else
		return (0);
}
/*#include <stdio.h>
#include <ctype.h>

int	main(void)
{
	int a;
	int b;
	
	a = isalnum('~');
	b = ft_isalnum('~');
	printf("test = %d \n mine = %d", a, b);
	return (0);
}*/
