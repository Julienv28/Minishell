/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:30:38 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/23 12:20:35 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcat(char *dest, char *src)
{
    int destlen;
    int i;

    i = 0;
    destlen = ft_strlen(dest);
    while (src[i])
    {
        dest[destlen + i] = src[i];
        i++;
    }
    dest[destlen + i] = 0;
    return (dest);
}
