/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 09:00:40 by opique            #+#    #+#             */
/*   Updated: 2025/06/13 09:54:43 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*path_error_message(char *cmd)
{
	if (access(cmd, F_OK) != 0)
	{
		printf("minishell: %s : No such file or directory\n", cmd);
		g_exit_status = 127;
		return (NULL);
	}
	if (is_directory(cmd))
	{
		printf("minishell: %s : is a directory\n", cmd);
		g_exit_status = 126;
		return (NULL);
	}
	if (access(cmd, X_OK) != 0)
	{
		printf("minishell: %s : Permission denied\n", cmd);
		g_exit_status = 126;
		return (NULL);
	}
	return (ft_strdup(cmd));
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		line;

	if (ft_strchr(cmd, '/'))
		return ((path_error_message(cmd)));
	line = find_line(envp, "PATH");
	if (line == -1 || !envp[line])
		return (ft_putstr_fd("minishell: No such file or directory\n", \
			STDERR_FILENO), NULL);
	paths = ft_split(envp[line] + 5, ':');
	path = search_path(paths, cmd);
	if (path == NULL)
	{
		printf("minishell: %s : command not found\n", cmd);
		g_exit_status = 127;
	}
	else if (access(path, X_OK) != 0)
	{
		printf("minishell: %s : Permission denied\n", cmd);
		g_exit_status = 126;
		free(path);
		path = NULL;
	}
	return (free_tab(paths), path);
}

void	exec_cmd(char **args, char ***envcp)
{
	char	*path;

	path = get_path(args[0], *envcp);
	if (path == NULL)
		exit(g_exit_status);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, args, *envcp) == -1)
	{
		if (errno == EISDIR)
		{
			printf("minishell: %s : Is a directory\n", path);
			g_exit_status = 126;
		}
		else
		{
			perror("minishell");
			g_exit_status = 1;
		}
		free(path);
		exit(g_exit_status);
	}
	exit(1);
}
