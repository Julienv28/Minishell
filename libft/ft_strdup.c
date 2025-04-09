/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:32:32 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/09 11:11:54 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strdup(char *src)
{
    char *str;
    char *p;
    int size;

    size = ft_strlen((char const *)src);
    str = malloc(size + 1);
    if (!str)
        return (NULL);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return (str);
}
