/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 10:59:34 by opique           ###   ########.fr       */
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

int	ft_pwd(char **args, char ***envcp)
{
	char	path[1024];

	(void)args;
	(void)envcp;
	if (args[1] && args[1][0] == '-' && args[1][1] != '\0')
	{
		printf("minishell: pwd: -%c: invalid option\n", args[1][1]);
		g_exit_status = 2;
		return (g_exit_status);
	}
	if (getcwd(path, sizeof(path)) != NULL)
		return (printf("%s\n", path), 0);
	else
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory: \
			getcwd: cannot access parent directories: \n", STDERR_FILENO);
		return (1);
	}
}
