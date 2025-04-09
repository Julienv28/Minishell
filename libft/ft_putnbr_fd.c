/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:24:55 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/09 17:17:04 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
void ft_putchar_fd(char c, int fd)
{
    write(fd, &c, 1);
}*/

void ft_putnbr_fd(int n, int fd)
{
    long int a;

    a = n;
    if (a < 0)
    {
        ft_putchar_fd('-', fd);
        a *= (-1);
    }
    if (a >= 00 && a <= 9)
    {
        ft_putchar_fd(a + '0', fd);
    }
    if (a > 9)
    {
        ft_putnbr_fd(a / 10, fd);
        ft_putnbr_fd(a % 10, fd);
    }
}

/*#include <stdlib.h>

int	main(int ac, char **av)
{
    if (ac < 2)
        return (0);
    ft_putnbr_fd(atoi(av[1]), 1);
    return (0);
}*/
