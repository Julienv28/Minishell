/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/03 14:27:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>

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
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args && args[0] ? args[0] : "", STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		g_exit_status = 127;
		return (-1);
	}
	if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
		ft_exit(args, 0);
	else if (is_builting(args[0]))
	{
		status = exec_builting(args, envcp);
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
        if (pid == 0) // Enfant
        {
            exec_cmd(args, envcp);
            exit(g_exit_status);
        }
        else // Parent
        {
            int status;
            // Ignorer SIGTSTP dans les processus enfants
            // signal(SIGTSTP, SIG_IGN);
            // Le parent ignore temporairement les signaux pendant que l'enfant s'exécute
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);

            waitpid(pid, &status, 0);

            // Réactiver les handlers après l'exécution de l'enfant
            signal(SIGINT, handler_sigint);
            signal(SIGQUIT, SIG_IGN);

            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
            {
                g_exit_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGINT)
                    write(1, "\n", 1); // pour garder comportement visuel de bash
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

/*
if (!args[1]) = Aucun argument -> exit 0
exit(255); = quitter avec code d'erreur, comme Bash
2 dernieres lignes = Convertir et tronquer à 8 bits comme Bash
*/
void	fake_exit_builtin(char **args)
{
	long long	exit_value;

	exit_value = 0;
	if (!args[1])
		exit(0);
	if (!is_valid_numeric_argument(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", \
				args[1]);
		exit(255);
	}
	exit_value = ft_atoull(args[1]);
	exit((unsigned char)(exit_value));
}

// void	exec_pipes(t_com_list *cmds, char ***envcp)
// {
// 	int			pipefd[2] = {-1, -1};
//     int			prev_fd = -1; // read end du pipe précédent
//     pid_t		pid;
//     t_com_list	*curr = cmds;

// 	while (curr)
// 	{
// 		if (curr->next)
// 		{
// 			if (pipe(pipefd) == -1)
// 			{
// 				perror("pipe");
// 				return ;
// 			}
// 		}
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			return ;
// 		}
// 		if (pid == 0) // === CHILD ===
// 		{
// 			printf("DEBUG: PID %d - Start of child process\n", getpid());
// 			// Redirection de l'entrée depuis le pipe précédent
// 			if (prev_fd >= 0)
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 				printf("DEBUG: PID %d - Redirection de l'entrée depuis le pipe précédent\n", getpid());
// 			}
//             // Redirection de la sortie vers le pipe suivant
// 			if (curr->next && pipefd[1] >= 0)
// 			{
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				printf("DEBUG: PID %d - Redirection de la sortie vers le pipe suivant\n", getpid());
// 			}
// 			if (pipefd[0] >= 0)
// 				close(pipefd[0]);
// 			if (pipefd[1] >= 0)
// 				close(pipefd[1]);
// 			if (curr->infile != NULL || curr->outfile != NULL
// 				|| curr->errfile != NULL || curr->heredoc_fd > 0)
// 			{
// 				printf("DEBUG: PID %d - Détection heredoc, redirection de l'entrée\n", getpid());
// 				if (ft_redirection(curr, NULL, NULL, NULL) < 0)
// 				{
// 					perror("redirection");
// 					exit(1);
// 				}
// 			}
//             // Préparation et exécution de la commande
// 			printf("DEBUG: PID %d - Découpage des arguments: %s\n", getpid(), curr->command);
// 			char **args = split_args(curr->command, ' ');
// 			if (!args || !args[0])
// 				exit(1);
// 			if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
// 				fake_exit_builtin(args);
// 			else if (is_builting(args[0]))
// 				exec_builting(args, envcp);
// 			else
// 				exec_cmd(args, envcp);
// 			free_tab(args);
// 			exit(g_exit_status);
// 		}
// 		else // === PARENT ===
// 		{
// 			// Fermer le read end précédent s’il existe
// 			if (prev_fd >= 0)
// 				close(prev_fd);
//             // Préparer le read end pour la prochaine commande
// 			if (curr->next)
// 			{
// 				if (pipefd[1] >= 0)
// 					close(pipefd[1]); // Écriture plus nécessaire
//                 prev_fd = pipefd[0]; // Lecture pour le prochain enfant
// 			}
// 			else
// 			{
// 				if (pipefd[0] >= 0)
// 					close(pipefd[0]);
// 				if (pipefd[1] >= 0)
// 					close(pipefd[1]);
// 			}
// 		}
// 		curr = curr->next;
// 		pipefd[0] = -1;
// 		pipefd[1] = -1;
// 	}
// 	wait_children();
// }

int exec_pipes(t_com_list *cmds, char **envcp)
{
    t_com_list *curr = cmds;
    pid_t pid;
    int pipefd[2];
    int prev_fd = -1;
    pid_t last_pid = -1;

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
            // Ignorer SIGTSTP dans les processus enfants
            signal(SIGTSTP, SIG_IGN);
            // Processus enfant : rétablir comportement par défaut pour SIGINT et SIGQUIT
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
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
            char **args = curr->args;
            if (!args || !args[0] || args[0][0] == '\0')
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(args && args[0] ? args[0] : "", STDERR_FILENO);
                ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
                exit(127);
            }
            if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
                fake_exit_builtin(args);
            else if (is_builting(args[0]))
            {
                int status = exec_builting(args, &envcp);
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
