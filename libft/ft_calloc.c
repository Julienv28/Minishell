/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:05:24 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/29 16:08:30 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;

	res = malloc(nmemb * size);
	if (!res)
		return (NULL);
	ft_bzero(res, nmemb * size);
	return (res);
}

/*#include <stdio.h>
int	 main(void) 
{
    int i; 
	int	*ptr;
	int	sum = 0;

	i = 0;
    ptr = ft_calloc(10, sizeof(int));
    if (ptr == NULL) 
	{
        printf("Error! memory not allocated.");
        exit(0);
    }
    printf("Building and calculating the sequence sum of the first 10 terms\n");
    while (i < 10) 
	{ 
		* (ptr + i) = i;
		sum += * (ptr + i);
		i ++;
    }
    printf("Sum = %d", sum);
    free(ptr);
    return 0;
}*/
