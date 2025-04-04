/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:32:32 by juvitry           #+#    #+#             */
/*   Updated: 2025/03/28 13:16:53 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strdup(const char *src)
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
