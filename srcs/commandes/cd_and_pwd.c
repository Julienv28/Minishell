/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_and_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/02 11:41:35 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_cd(char **args, char ***envcp)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*path = NULL;
	char	*home = get_env_value("HOME", *envcp);
	size_t	home_len;	
	size_t	suffix_len;

	if (!getcwd(current_dir, sizeof(current_dir)))
		current_dir[0] = '\0';
	if (args[1] && args[1][0] == '\0')
	{
    	if (!home)
    	{
        	ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
        	return (1);
		}
	}
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!args[1] || ft_strcmp(args[1], "--") == 0)
	{
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
		path = home;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value("OLDPWD", *envcp);
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
		printf("%s\n", path);
	}
	else if (args[1][0] == '-' && ft_strcmp(args[1], "-") != 0 && ft_strcmp(args[1], "--") != 0)
	{
		printf("minishell: cd: %s: invalid option\n", args[1]);
		g_exit_status = 2;
		return (g_exit_status);
	}
	else if (args[1][0] == '~')
	{
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
		home_len = ft_strlen(home);
		suffix_len = ft_strlen(args[1] + 1);
		path = malloc(home_len + suffix_len + 1);
		if (!path)
			return (1);
		ft_strcpy(path, home);
		ft_strcat(path, args[1] + 1);
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s : No such file or directory\n", path);
		return (1);
	}
	if (!getcwd(new_dir, sizeof(new_dir)))
	{
		ft_putstr_fd("chdir: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
		perror("");
		return (1);
	}
	if (current_dir[0] != '\0')
		ft_set_env("OLDPWD", current_dir, envcp);
	ft_set_env("PWD", new_dir, envcp);
	if (args[1] && args[1][0] == '~')
		free(path);
	return (0);
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
