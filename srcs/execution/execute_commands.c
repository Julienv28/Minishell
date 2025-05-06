/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/06 13:41:37 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Redirige ;es entrees/sorties du processus enfant
static void	setup_child_io(int prev_fd, int *fd, int has_next)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

static void	hand_child_proc(t_com_list *cmd, int prev_fd, int *fd, int has_next)
{
	setup_child_io(prev_fd, fd, has_next);
	if (cmd->infile || cmd->outfile)
	{
		int	redir_fd = ft_redirection(cmd);
		if (redir_fd == -1)
			exit(1);
	}
	if (cmd->args && is_builting(cmd->args[0]) == 0)
		exec_builting(cmd->args, &cmd->envcp);
	else
		exec_cmd(cmd);
	exit(g_exit_status);
}

static int	hand_parent_proc(pid_t pid, int prev_fd, int *fd, int has_next)
{
	int	status;

	if (prev_fd != -1)
		close(prev_fd);
	if (has_next)
	{
		close(fd[1]);
		prev_fd = fd[0];
	}
	else if (fd[0] >= 0)
		close (fd[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	if (has_next)
		return (fd[0]);
	else
		return (-1);
}

void	execute_cmd(t_com_list *cmd)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		has_next;

	prev_fd = -1;
	while (cmd)
	{
		has_next = (cmd->next != NULL);
		if (has_next && pipe(fd) == -1)
			exit_error();
		pid = fork();
		if (pid == -1)
			exit_error();
		else if (pid == 0)
			hand_child_proc(cmd, prev_fd, fd, has_next);
		prev_fd = hand_parent_proc(pid, prev_fd, fd, has_next);
		cmd = cmd->next;
	}
}
