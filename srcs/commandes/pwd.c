/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/04 11:18:54 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	{
		printf("%s\n", path);
		return (0);
	}
	else
	{
		char *pwd = get_env_value("PWD", *envcp);
		if (pwd)
		{
			printf("%s\n", pwd);
			return (0);
		}
		ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
		return (1);
    }
}
