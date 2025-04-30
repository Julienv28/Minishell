/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:02:35 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/30 09:50:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	child_proc(t_com_list *command, char **args, char **envcp, int *fd)
{
	int	filein;

	filein = open(args[1], O_RDONLY);
	if (filein == -1)
		exit_error();
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (args && is_builting(args[0]) == 0)
                exec_builting(args, &envcp);
            else
                exec_cmd(command, envcp);
}

static void	parent_proc(t_com_list *command, char **args, char **envcp, int *fd)
{
	int	fileout;

	fileout = open(args[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		exit_error();
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	if (args && is_builting(args[0]) == 0)
                exec_builting(args, &envcp);
            else
                exec_cmd(command, envcp);
}

static void	timeout_handling(pid_t pid)
{
	int	status;
	int	i;

	i = 0;
	while (i < TIMEOUT_ITERATIONS)
	{
		if (waitpid(pid, &status, WNOHANG) > 0)
			return ;
		i++;
	}
	ft_putstr_fd("Error Timeout. Processed Killed.\n", 2);
	exit(0);
}

void	pipex_simple(t_com_list *command, char **args, char **envcp)
{
	int		fd[2];
	pid_t	pid1;

	if (pipe(fd) == -1)
		exit_error();
	pid1 = fork();
	if (pid1 == -1)
		exit_error();
	if (pid1 == 0)
		child_proc(command, args, envcp, fd);
	timeout_handling(pid1);
	parent_proc(command, args, envcp, fd);
}
