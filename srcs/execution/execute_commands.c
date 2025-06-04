/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/04 16:25:12 by juvitry          ###   ########.fr       */
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
