/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/13 16:05:43 by juvitry          ###   ########.fr       */
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

int	is_builtin_global(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}

void execute(t_com_list *cmds, char **envcp)
{
    char **args;
    pid_t pid;

    if (!cmds)
        return;
    if (cmds->next == NULL)  // Une seule commande
    {
        args = split_args(cmds->command, ' ');
        if (!args || !args[0])
        {
            free_tab(args);
            return;
        }
        replace_exit_and_env_status(args, envcp);
        // Vérifie si c'est un builtin
        if (is_builting(args[0])) 
        {
            if (is_builtin_global(args[0]))  // Si c'est un builtin global (ex: exit, cd, export, unset)
                exec_builting(args, &envcp);  // Exécution dans le processus parent
            else  // Si c'est un autre builtin (pas global)
            {
                pid = fork();
                if (pid == -1)
                {
                    perror("fork");
                    free_tab(args);
                    return;
                }
                if (pid == 0)  // === Processus Enfant ===
                {
                    exec_builting(args, &envcp);  // Exécution du builtin dans l'enfant
                    exit(g_exit_status);
                }
                else  // === Processus Parent ===
                {
                    int status;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                        g_exit_status = WEXITSTATUS(status);
                    else if (WIFSIGNALED(status))
                        g_exit_status = 128 + WTERMSIG(status);
                }
            }
        }
        else  // Si ce n'est pas un builtin
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                free_tab(args);
                return;
            }
            if (pid == 0)  // === Processus Enfant ===
            {
                exec_cmd(cmds, envcp);  // Exécution de la commande externe
                exit(g_exit_status);
            }
            else  // === Processus Parent ===
            {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    g_exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    g_exit_status = 128 + WTERMSIG(status);
            }
        }
        free_tab(args);
    }
    else  // Si il y a plusieurs commandes (avec pipes)
        exec_pipes(cmds, envcp);
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

void exec_pipes(t_com_list *cmds, char **envcp)
{
    int prev_fd = -1;
    int pipefd[2];
    pid_t pid;
    t_com_list *curr = cmds;

    while (curr)
    {
        if (curr->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
        }
        printf("Creating process for command: %s\n", curr->command);
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
        }
        if (pid == 0)  // === CHILD ===
        {
            printf("Child process %d: %s\n", getpid(), curr->command);
            if (prev_fd != -1)
            {
                printf("Child %d: Duplicating stdin from fd %d\n", getpid(), prev_fd);
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (curr->next)
            {
                printf("Child %d: Duplicating stdout to fd %d\n", getpid(), pipefd[1]);
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            char **args = split_args(curr->command, ' ');
            if (!args || !args[0])
            {
                printf("Invalid or empty command: %s\n", curr->command);
                exit(1);
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
        {
            printf("Parent: Closing fd %d\n", prev_fd);
            close(prev_fd);
        }
        if (curr->next)
        {
            printf("Parent: Closing fd %d\n", pipefd[1]);
            close(pipefd[1]);
        }
		prev_fd = (curr->next) ? pipefd[0] : -1;
        curr = curr->next;
    }
    wait_children();
}
