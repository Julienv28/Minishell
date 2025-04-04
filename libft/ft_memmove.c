/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:09:37 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/29 16:08:26 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char			*pdest;
	const unsigned char		*psrc;
	size_t					i;

	pdest = (unsigned char *)dest;
	psrc = (const unsigned char *)src;
	i = 0;
	if (pdest > psrc && pdest < psrc + n)
	{
		while (n > 0)
		{
			n--;
			pdest[n] = psrc[n];
		}
	}
	else
	{
		while (i < n)
		{
			pdest[i] = psrc[i];
			i++;
		}
	}
	return (dest);
}
//#include <stdio.h>

/*int	main(void)
{
	char		dest[39] = "Human Centipede 2 est mon film prefere";
	const char	src[17] = "Lawrence Anyways";
	
	printf("Before ft_memmove >> dest = %s, src = %s\n\n", dest, src);
	ft_memmove(dest + 18, src, 17);
	printf("After ft_memmove >> dest = %s, src = %s\n\n", dest, src);
	return (0);
}*/

/*int main()
{
    char str[50] = "I am going from Delhi to Gorakhpur";
    printf( "Function:\tft_memmove with overlap\n" );
    printf( "Orignal :\t%s\n",str);
    printf( "Source:\t\t%s\n", str + 5 );
    printf( "Destination:\t%s\n", str + 11 );
    ft_memmove( str + 11, str + 5, 29 );
    printf( "Result:\t\t%s\n", str );
    return 0;
}*/
