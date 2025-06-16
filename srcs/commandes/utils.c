/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:33:41 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*export_s(char *arg, t_msh *msh, char **key, char **value)
{
	char	*equal;
	char	*replaced;

	*key = NULL;
	*value = NULL;
	replaced = replace_var(arg, msh, 0, 1);
	if (!replaced)
		return (NULL);
	equal = ft_strchr(replaced, '=');
	if (equal)
	{
		*key = ft_substr(replaced, 0, equal - replaced);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(replaced);
		*value = NULL;
	}
	return (replaced);
}

void	print_exit_error(char *arg, char *msg, int fd)
{
	ft_putstr_fd("minishell: exit: ", fd);
	if (arg)
		ft_putstr_fd(arg, fd);
	ft_putstr_fd(msg, fd);
	ft_putstr_fd("\n", fd);
}

void	print_cmd_error(char *cmd, char *msg, int fd)
{
	ft_putstr_fd("minishell: ", fd);
	ft_putstr_fd(cmd, fd);
	ft_putstr_fd(": ", fd);
	ft_putstr_fd(msg, fd);
	ft_putstr_fd("\n", fd);
}

void	print_fd(char *msg, char c, int repeat, int fd)
{
	int	k;

	ft_putstr_fd(msg, fd);
	k = 0;
	while (k < repeat)
	{
		ft_putchar_fd(c, fd);
		k++;
	}
	ft_putstr_fd("'\n", fd);
}

void	print_error_token_fd(char *msg, int fd)
{
	ft_putstr_fd(msg, fd);
	ft_putstr_fd("\n", fd);
}
