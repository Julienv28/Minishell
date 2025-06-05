/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:53:38 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 11:30:43 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	child_exec(char **args, char ***envcp)
{
	exec_cmd(args, envcp);
	exit(g_exit_status);
}

static void	parent_wait(pid_t pid, int *status)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, status, 0);
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static int	handle_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (-1);
}

int	exec_external(char **args, char ***envcp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		child_exec(args, envcp);
	parent_wait(pid, &status);
	return (handle_exit_status(status));
}
