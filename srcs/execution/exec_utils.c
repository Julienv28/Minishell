/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:31:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 10:47:41 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_ags(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

int	find_line(char **envp, char *path)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(path, envp[i], ft_strlen(path)) == 0
			&& envp[i][ft_strlen(path)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*search_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*part_path;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (0);
}

int	is_valid_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
