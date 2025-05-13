/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/13 15:14:34 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

// // Redirige ;es entrees/sorties du processus enfant
// static void setup_child_io(int prev_fd, int *fd, int has_next)
// {
//     if (prev_fd != -1)
//     {
//         dup2(prev_fd, STDIN_FILENO);
//         close(prev_fd);
//     }
//     if (has_next)
//     {
//         close(fd[0]);
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[1]);
//     }
// }

// static void hand_child_proc(t_com_list *cmd, int prev_fd, int *fd, int has_next)
// {
//     setup_child_io(prev_fd, fd, has_next);
//     if (cmd->args && is_builting(cmd->args[0]) == 0)
//         exec_builting(cmd->args, &cmd->envcp);
//     else
//         exec_cmd(cmd, envcp);
//     exit(g_exit_status);
// }

// static int hand_parent_proc(pid_t pid, int prev_fd, int *fd, int has_next)
// {
//     int status;

//     if (prev_fd != -1)
//         close(prev_fd);
//     if (has_next)
//     {
//         close(fd[1]);
//         prev_fd = fd[0];
//     }
//     else if (fd[0] >= 0)
//         close(fd[0]);
//     waitpid(pid, &status, 0);
//     if (WIFEXITED(status))
//         g_exit_status = WEXITSTATUS(status);
//     else if (WIFSIGNALED(status))
//         g_exit_status = 128 + WTERMSIG(status);
//     if (has_next)
//         return (fd[0]);
//     else
//         return (-1);
// }

// void execute_cmd(t_com_list *cmd)
// {
//     int fd[2];
//     int prev_fd;
//     pid_t pid;
//     int has_next;

//     prev_fd = -1;
//     while (cmd)
//     {
//         has_next = (cmd->next != NULL);
//         if (has_next && pipe(fd) == -1)
//             exit_error();
//         pid = fork();
//         if (pid == -1)
//             exit_error();
//         else if (pid == 0)
//             hand_child_proc(cmd, prev_fd, fd, has_next);
//         prev_fd = hand_parent_proc(pid, prev_fd, fd, has_next);
//         cmd = cmd->next;
//     }
// }

static void	read_com_list(t_com_list *cmds)
{
	t_com_list *tmp = cmds;
	int i = 0;
	while (tmp)
	{
    	printf("Command %d: %s\n", i++, tmp->command);
    	tmp = tmp->next;
	}
}

void	execute(t_com_list *cmds, char **envcp)
{
	char	**args;
	pid_t	pid;

	if (!cmds)
		return ;
	if (cmds->next == NULL)
	{
		args = split_args(cmds->command, ' ');
		if (!args || !args[0])
		{
			free_tab(args);
			return ;
		}
		replace_exit_and_env_status(args, envcp);
		if (ft_strcmp(args[0], "exit") == 0)
			exec_builting(args, &envcp);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_tab(args);
			return ;
		}
		if (pid == 0) // === CHILD ===
		{
			if (is_builting(args[0]) == 0)
				exec_builting(args, &envcp);
			else
				exec_cmd(cmds, envcp);
			free_tab(args);
			exit(g_exit_status);
		}
		else // === PARENT ===
		{
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
		}
		free_tab(args);
	}
	else
	{
		read_com_list(cmds);
		exec_pipes(cmds, envcp);
	}
}

// static int	count_commands(t_com_list *cmds)
// {
// 	int			count;
// 	t_com_list	*tmp;

// 	count = 0;
// 	tmp = cmds;
// 	while (tmp)
// 	{
// 		count++;
// 		tmp = tmp->next;
// 	}
// 	return (count);
// }

static void	wait_children(void)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid != -1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		pid = wait(&status);
	}
	if (errno != ECHILD)
		perror("wait");
}

void	exec_pipes(t_com_list *cmds, char **envcp)
{
	int			prev_fd = -1;
	int 		pipefd[2];
	pid_t		pid;
	t_com_list	*curr = cmds;

	while (curr)
	{
		if (curr->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		printf("creating process for command: %s\n", curr->command);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)  // === CHILD ===
		{
			printf("Child process %d: %s\n", getpid(), curr->command);
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
			char **args = split_args(curr->command, ' ');
			if (!args || !args[0])
			{
				printf("Invalid or empty command: %s\n", curr->command);
				exit (1);
			}
			replace_exit_and_env_status(args, envcp);
			if (is_builting(args[0]))
				exec_builting(args, &envcp);
			else
				exec_cmd(curr, envcp);
			free_tab(args);
			exit(g_exit_status);
		}
		// === PARENT ===
		if (prev_fd != -1)
			close(prev_fd);
		if (curr->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		else
		{
			if (pipefd[0] >= 0)
				close(pipefd[0]);
			if (pipefd[1] >= 0)
				close(pipefd[1]);
			prev_fd = -1;
		}
		curr = curr->next;
	}
	wait_children();
}
