/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:02:35 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/16 16:44:07 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	child_process(char **av, char **envp, int *fd)
{
	int	filein;

	filein = open(av[1], O_RDONLY);
	if (filein == -1)
		exit_error();
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_exec(av[2], envp);
}

static void	parent_process(char **av, char **envp, int *fd)
{
	int	fileout;

	fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		exit_error();
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	ft_exec(av[3], envp);
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

void	pipex_simple(char **av, char **envp)
{
	int		fd[2];
	pid_t	pid1;

	if (pipe(fd) == -1)
		exit_error();
	pid1 = fork();
	if (pid1 == -1)
		exit_error();
	if (pid1 == 0)
		child_process(av, envp, fd);
	timeout_handling(pid1);
	parent_process(av, envp, fd);
}
