/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:51:24 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/26 12:02:18 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*// ce main ne fera qu'ecrire la premiere lettre de l'argument passe en av[1].
int	main(int ac, char **av)
{
    if (ac < 2)
        return (0);
    ft_putchar_fd(av[1][0], 1);
    // 1 correspond a la sortie terminal.
}*/