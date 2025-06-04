/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/04 14:11:19 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute(t_com_list *cmds, char ***envcp)
{
	char	**args;
	pid_t	pid;
	int		status;

	if (!cmds)
		return (-1);
	args = cmds->args;
	if (!args || !args[0] || args[0][0] == '\0')
	{
		// ft_putstr_fd("minishell: ", STDERR_FILENO);
		// ft_putstr_fd(args && args[0] ? args[0] : "", STDERR_FILENO);
		// ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		//g_exit_status = 127;
		//free_tab(args);
		g_exit_status = 0;
		return (0);
	}
	if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
	{
		ft_freeenvp(*envcp);
    	rl_clear_history();
		ft_exit(args, 0, cmds);
	}
	else if (is_builting(args[0]))
	{
		status = exec_builting(args, envcp, cmds);
			g_exit_status = status;
	}
    else
    {
        // FORK pour les commandes externes
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return (-1);
        }
        if (pid == 0)
        {
            exec_cmd(args, envcp);
            exit(g_exit_status);
        }
        else
        {
            int status;
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);

            waitpid(pid, &status, 0);

            signal(SIGINT, handler_sigint);
            signal(SIGQUIT, SIG_IGN);

            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
            {
                g_exit_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGINT)
                    write(1, "\n", 1);
                else if (WTERMSIG(status) == SIGQUIT)
                    write(1, "Quit (core dumped)\n", 20);
            }
        }
    }
    return (g_exit_status);
}

static void	wait_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	if (WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
}

void	fake_exit_builtin(char **args, t_com_list *cmds)
{
	long long	exit_value;

	exit_value = 0;
	if (!args[1])
		cleanup_and_exit(0, cmds);
	if (!is_valid_numeric_argument(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", \
				args[1]);
		cleanup_and_exit(255, cmds);
	}
	exit_value = ft_atoull(args[1]);
	exit((unsigned char)(exit_value));
}

int	exec_pipes(t_com_list *cmds, char **envcp)
{
	t_com_list	*curr = cmds;
	pid_t 		pid;
	int 		pipefd[2];
	int 		prev_fd = -1;
	pid_t 		last_pid = -1;
	char		**args;
	int			status;

	while (curr)
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
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return (-1);
		}
		if (pid == 0) // ---CHILD---
		{
			signal(SIGTSTP, SIG_IGN);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (ft_redirection(curr, NULL, NULL, NULL) < 0)
        		exit(1); // erreur ouverture fichier
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
			if (!args || !args[0] || args[0][0] == '\0')
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(args && args[0] ? args[0] : "", STDERR_FILENO);
				ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
				rl_clear_history();
				exit(127);
			}
			if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
			{
				ft_freeenvp(envcp);
				fake_exit_builtin(args, cmds);
			}
			else if (is_builting(args[0]))
			{
				status = exec_builting(args, &envcp, cmds);
				g_exit_status = status;
			}
			else
				exec_cmd(args, &envcp);
			exit(g_exit_status);
		}
        //---PARENT---
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (prev_fd != -1)
			close(prev_fd);
		if (curr->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		else
		{
			last_pid = pid;
			if (pipefd[0] != -1)
				close(pipefd[0]);
			prev_fd = -1;
		}
		curr = curr->next;
	}
	wait_children(last_pid);
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (g_exit_status);
}
