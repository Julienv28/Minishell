/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 15:52:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	if (!name || !envp)
		return (NULL);
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (envp[i] && ft_strncmp(envp[i], name, len) == 0
			&& envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	ft_pwd(char **args, t_msh *msh)
{
	char	path[1024];

	(void)args;

	if (args[1] && args[1][0] == '-' && args[1][1] != '\0')
	{
		ft_putstr_fd("minishell: pwd: -", STDERR_FILENO);
		ft_putchar_fd(args[1][1], STDERR_FILENO);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
		msh->ex_status = 2;
		return (msh->ex_status);
	}
	if (getcwd(path, sizeof(path)) != NULL)
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory: \
			getcwd: cannot access parent directories: \n", STDERR_FILENO);
		return (1);
	}
}

// pwd = get_env_value("PWD", msh->envcp);
// 		if (pwd)
// 			return (printf("%s\n", pwd), 0);
