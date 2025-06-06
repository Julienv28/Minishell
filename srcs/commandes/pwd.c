/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pique <pique@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 15:39:02 by pique            ###   ########.fr       */
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
	{
		printf("[DEBUG] get_env_value: envp is NULL\n");
		return (NULL);
	}
		
	while (envp[i])
	{
		if (envp[i] && ft_strncmp(envp[i], name, len) == 0
			&& envp[i][len] == '=')
		{
			printf("[DEBUG] get_env_value: found '%s' → '%s'\n", name, envp[i] + len + 1);
			return (envp[i] + len + 1);
		}
		i++;
	}
	printf("[DEBUG] get_env_value: '%s' not found in envp\n", name);
	return (NULL);
}

// PWD
// Affiche le chemin absolu du répertoire courant.
int ft_pwd(char **args, char ***envcp)
{
	(void)args;
    char    path[1024];

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
		char *pwd = get_env_value("PWD", *envcp);
		if (pwd)
			return (printf("%s\n", pwd), 0);
		ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
		return (1);
    }
}
