/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:03:35 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:43:57 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_pipes(t_com *cmds, t_msh *msh)
{
	t_com	*curr;
	int		prev_fd;
	pid_t	last_pid;
	pid_t	pid;

	curr = cmds;
	prev_fd = -1;
	last_pid = -1;
	while (curr)
	{
		pid = exec(curr, &prev_fd, &last_pid, msh);
		if (pid < 0)
			return (-1);
		curr = curr->next;
	}
	wait_children(last_pid);
	reset_signals();
	return (msh->ex_status);
}

int	exec(t_com *curr, int *prev, pid_t *lst_pid, t_msh *msh)
{
	t_execinfo	ex;

	ex.curr = curr;
	ex.prev_fd = prev;
	ex.last_pid = lst_pid;
	if (setup_pipe(curr, ex.pipefd) < 0)
		return (-1);
	ex.pid = fork();
	if (ex.pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (ex.pid == 0)
		child(curr, *prev, ex.pipefd, msh);
	parent_pro(&ex);
	return (ex.pid);
}

int	setup_pipe(t_com *curr, int pipefd[2])
{
	if (curr->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (0);
}

void	parent_pro(t_execinfo *ex)
{
	set_signals_parent();
	if (*(ex->prev_fd) != -1)
		close(*(ex->prev_fd));
	if (ex->curr->next)
	{
		close(ex->pipefd[1]);
		*(ex->prev_fd) = ex->pipefd[0];
	}
	else
	{
		*(ex->last_pid) = ex->pid;
		if (ex->pipefd[0] != -1)
			close(ex->pipefd[0]);
		*(ex->prev_fd) = -1;
	}
}

void	child(t_com *curr, int prev, int pipefd[2], t_msh *msh)
{
	char	**args;

	set_signals_child();
	if (ft_redir(curr, NULL, NULL, NULL) < 0)
		exit(1);
	if (prev != -1)
	{
		dup2(prev, STDIN_FILENO);
		close(prev);
	}
	if (curr->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	args = curr->args;
	handle_pipes(args, curr, msh);
	exit(msh->ex_status);
}
