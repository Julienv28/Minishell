/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:23:53 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/12 14:07:50 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*pdest;
	const char		*psrc;

	pdest = dest;
	psrc = (const char *)src;
	while (n--)
		*pdest++ = *psrc++;
	return (dest);
}
/*#include <stdio.h>

int	main(void)
{
	char src[20] = "How Are you ?";
    char dst[20] = {0}; 
    
    ft_memcpy(dst,src,sizeof(src));
    printf("dst = %s\n", dst);
    return (0);
}*/
