/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:12:31 by juvitry           #+#    #+#             */
/*   Updated: 2024/11/24 16:10:12 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write(fd, s, i);
	write(fd, "\n", 1);
}

// int	main(int ac, char **av)
// {
// 	int	i;
// 	if (ac < 2)
// 	{
// 		ft_putendl_fd("Ajoutez des arguments", 1);
// 		return (0);
// 	}
// 	i = 1;
// 	while (i < ac)
// 	{
// 		ft_putendl_fd(av[i], 1);
// 		i++;
// 	}
// 	return (0);
// }
