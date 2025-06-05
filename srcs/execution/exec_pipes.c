/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:03:35 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 11:24:45 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	exec_pipes(t_com_list *cmds, char **envcp)
// {
// 	t_com_list	*curr = cmds;
// 	pid_t 		pid;
// 	int 		pipefd[2];
// 	int 		prev_fd = -1;
// 	pid_t 		last_pid = -1;
// 	char		**args;
// 	int			status;

// 	while (curr)
// 	{
// 		if (curr->next)
// 		{
// 			if (pipe(pipefd) == -1)
// 			{
// 				perror("pipe");
// 				return (-1);
// 			}
// 		}
// 		else
// 		{
// 			pipefd[0] = -1;
// 			pipefd[1] = -1;
// 		}
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			perror("fork");
// 			return (-1);
// 		}
// 		if (pid == 0) // ---CHILD---
// 		{
// 			signal(SIGTSTP, SIG_IGN);
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			if (ft_redirection(curr, NULL, NULL, NULL) < 0)
//         		exit(1); // erreur ouverture fichier
// 			if (prev_fd != -1)
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 			}
// 			if (curr->next)
// 			{
// 				close(pipefd[0]);
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				close(pipefd[1]);
// 			}
// 			args = curr->args;
// 			if (!args || !args[0] || args[0][0] == '\0')
// 			{
// 				ft_putstr_fd("minishell: ", STDERR_FILENO);
// 				ft_putstr_fd(args && args[0] ? args[0] : "", STDERR_FILENO);
// 				ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 				rl_clear_history();
// 				exit(127);
// 			}
// 			if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
// 			{
// 				ft_freeenvp(envcp);
// 				fake_exit_builtin(args, cmds);
// 			}
// 			else if (is_builting(args[0]))
// 			{
// 				status = exec_builting(args, &envcp, cmds);
// 				g_exit_status = status;
// 			}
// 			else
// 				exec_cmd(args, &envcp);
// 			exit(g_exit_status);
// 		}
//         //---PARENT---
// 		signal(SIGINT, SIG_IGN);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (prev_fd != -1)
// 			close(prev_fd);
// 		if (curr->next)
// 		{
// 			close(pipefd[1]);
// 			prev_fd = pipefd[0];
// 		}
// 		else
// 		{
// 			last_pid = pid;
// 			if (pipefd[0] != -1)
// 				close(pipefd[0]);
// 			prev_fd = -1;
// 		}
// 		curr = curr->next;
// 	}
// 	wait_children(last_pid);
// 	signal(SIGINT, handler_sigint);
// 	signal(SIGQUIT, SIG_IGN);
// 	return (g_exit_status);
// }

int	exec_pipes(t_com_list *cmds, char **envcp)
{
	t_com_list	*curr;
	int			prev_fd;
	pid_t		last_pid;
	pid_t		pid;

	curr = cmds;
	prev_fd = -1;
	last_pid = -1;
	while (curr)
	{
		pid = fork_exec(curr, &prev_fd, &last_pid, &envcp);
		if (pid < 0)
			return (-1);
		curr = curr->next;
	}
	wait_children(last_pid);
	reset_signals();
	return (g_exit_status);
}

int	fork_exec(t_com_list *curr, int *prev_fd, pid_t *last_pid, char ***envcp)
{
	t_execinfo	ex;

	ex.curr = curr;
	ex.prev_fd = prev_fd;
	ex.last_pid = last_pid;
	if (setup_pipe(curr, ex.pipefd) < 0)
		return (-1);
	ex.pid = fork();
	if (ex.pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (ex.pid == 0)
		child_proc(curr, *prev_fd, ex.pipefd, envcp);
	parent_pro(&ex);
	return (ex.pid);
}

int	setup_pipe(t_com_list *curr, int pipefd[2])
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

void	child_proc(t_com_list *curr, int prev_fd, int pipefd[2], char ***envcp)
{
	char	**args;

	set_signals_child();
	if (ft_redirection(curr, NULL, NULL, NULL) < 0)
		exit(1);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (curr->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	args = curr->args;
	handle_commands_pipes(args, curr, envcp);
	exit(g_exit_status);
}
