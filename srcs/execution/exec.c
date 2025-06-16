/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 09:00:40 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 14:38:16 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*path_error_message(char *cmd, t_msh *msh)
{
	if (access(cmd, F_OK) != 0)
	{
		printf("minishell: %s : No such file or directory\n", cmd);
		msh->ex_status = 127;
		return (NULL);
	}
	if (is_directory(cmd))
	{
		printf("minishell: %s : is a directory\n", cmd);
		msh->ex_status = 126;
		return (NULL);
	}
	if (access(cmd, X_OK) != 0)
	{
		printf("minishell: %s : Permission denied\n", cmd);
		msh->ex_status = 126;
		return (NULL);
	}
	return (ft_strdup(cmd));
}

char	*get_path(char *cmd, t_msh *msh)
{
	char	**paths;
	char	*path;
	int		line;

	if (ft_strchr(cmd, '/'))
		return ((path_error_message(cmd, msh)));
	line = find_line(msh->envcp, "PATH");
	if (line == -1 || !msh->envcp[line])
		return (ft_putstr_fd("minishell: No such file or directory\n", \
			STDERR_FILENO), NULL);
	paths = ft_split(msh->envcp[line] + 5, ':');
	path = search_path(paths, cmd);
	if (path == NULL)
	{
		printf("minishell: %s : command not found\n", cmd);
		msh->ex_status = 127;
	}
	else if (access(path, X_OK) != 0)
	{
		printf("minishell: %s : Permission denied\n", cmd);
		msh->ex_status = 126;
		free(path);
		path = NULL;
	}
	return (free_tab(paths), path);
}

void	exec_cmd(char **args, t_msh *msh)
{
	char	*path;

	path = get_path(args[0], msh);
	if (path == NULL)
		exit(msh->ex_status);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, args, msh->envcp) == -1)
	{
		if (errno == EISDIR)
		{
			printf("minishell: %s : Is a directory\n", path);
			msh->ex_status = 126;
		}
		else
		{
			perror("minishell");
			msh->ex_status = 1;
		}
		free(path);
		exit(msh->ex_status);
	}
	exit(1);
}
