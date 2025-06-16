/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:21:13 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 13:26:15 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd_expand_home(char *arg, char **path, char *home)
{
	size_t	home_len;
	size_t	suffix_len;

	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	home_len = ft_strlen(home);
	suffix_len = ft_strlen(arg + 1);
	*path = malloc(home_len + suffix_len + 1);
	if (!*path)
		return (1);
	ft_strcpy(*path, home);
	ft_strcat(*path, arg + 1);
	return (0);
}

int	cd_change_dir(char *path, char *current_dir, char *new_dir, char ***envcp)
{
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s : No such file or directory\n", path);
		return (1);
	}
	if (!getcwd(new_dir, 1024))
	{
		ft_putstr_fd("chdir: error retrieving current directory: getcwd: \
			cannot access parent directories: ", STDERR_FILENO);
		perror("");
		return (1);
	}
	if (current_dir[0] != '\0')
		ft_set_env("OLDPWD", current_dir, envcp);
	ft_set_env("PWD", new_dir, envcp);
	return (0);
}

int	cd_get_path(char **args, char ***envcp, char **path)
{
	char	*home;

	home = get_env_value("HOME", *envcp);
	if (!args[1] || ft_strcmp(args[1], "--") == 0 || args[1][0] == '\0')
	{
		if (!home)
			return (ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO), 1);
		*path = home;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*path = get_env_value("OLDPWD", *envcp);
		if (!*path)
			return (ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO), 1);
		printf("%s\n", *path);
	}
	else if (args[1][0] == '~')
		return (cd_expand_home(args[1], path, home));
	else
		*path = args[1];
	return (0);
}

static int	cd_check_args(char **args, t_msh *msh)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (args[1] && args[1][0] == '-' && ft_strcmp(args[1], "-") != 0
		&& ft_strcmp(args[1], "--") != 0)
	{
		printf("minishell: cd: %s: invalid option\n", args[1]);
		msh->ex_status = 2;
		return (msh->ex_status);
	}
	return (0);
}

int	ft_cd(char **args, t_msh *msh)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*path;
	int		ret;

	path = NULL;
	if (!getcwd(current_dir, sizeof(current_dir)))
		current_dir[0] = '\0';
	ret = cd_check_args(args, msh);
	if (ret != 0)
		return (ret);
	ret = cd_get_path(args, &(msh->envcp), &path);
	if (ret != 0)
		return (ret);
	ret = cd_change_dir(path, current_dir, new_dir, &(msh->envcp));
	if (args[1] && args[1][0] == '~')
		free(path);
	return (ret);
}
