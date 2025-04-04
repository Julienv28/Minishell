/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:04:20 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/24 15:20:59 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		i++;
	write(fd, s, i);
}

// int	main(int ac, char **av)
// {
// 	if (ac < 2)
// 	{
// 		ft_putstr_fd("Ajoutez texte \n", 1);
// 		return (0);
// 	}
// 	ft_putstr_fd(av[1], 1);
// 	return (0);
// }
