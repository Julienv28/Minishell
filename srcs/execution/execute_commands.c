/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/15 19:37:24 by oceanepique      ###   ########.fr       */
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

/*
int is_builtin_global(char *cmd)
{
    return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0);
}*/

void execute(t_com_list *cmds, char ***envcp)
{
    char **args;
    pid_t pid;

    if (!cmds)
        return;
    // if (cmds->next == NULL) // Une seule commande
    //{
    args = split_args(cmds->command, ' ');
    if (!args || !args[0])
    {
        free_tab(args);
        return;
    }

    expand_variables(args, *envcp);

    if (is_builting(args[0]))
    {
        // PAS DE FORK pour les builtins
        exec_builting(args, envcp);
    }
    else
    {
        // FORK pour les commandes externes
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free_tab(args);
            return;
        }

        if (pid == 0) // Enfant
        {
            exec_cmd(cmds, envcp);
            exit(g_exit_status);
        }
        else // Parent
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
    //}
    // else
    // {
    //     exec_pipes(cmds, envcp);
    //     printf("\nOUT\n");
    // }
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

static void wait_children(void)
{
    int status;
    pid_t pid;

    pid = wait(&status);
    while (pid > 0) // Tant qu'il y a des processus enfants à attendre
    {
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_exit_status = 128 + WTERMSIG(status);
        pid = wait(&status);
    }
}

void exec_pipes(t_com_list *cmds, char ***envcp)
{
    int pipefd[2];
    int prev_fd = -1; // pour garder le read end du pipe précédent
    pid_t pid;
    t_com_list *curr = cmds;

    while (curr)
    {
        // Crée un pipe s'il y a une commande suivante
        if (curr->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
        }

        if (pid == 0) // === CHILD ===
        {
            // Si ce n'est pas la première commande : redirige l'entrée
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // Si ce n'est pas la dernière commande : redirige la sortie
            if (curr->next)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }
            else
            {
                close(pipefd[1]); // Fermer le côté écriture du pipe si c'est le dernier
                close(pipefd[0]); // Fermer le côté lecture du pipe
            }
            // Prépare les arguments
            char **args = split_args(curr->command, ' ');
            if (!args || !args[0])
                exit(1);

            expand_variables(args, *envcp);

            if (is_builting(args[0]))
                exec_builting(args, envcp); // forké même si builtin
            else
                exec_cmd(curr, envcp);

            free_tab(args);
            exit(g_exit_status); // Par sécurité
        }
        else // === PARENT ===
        {
            // Ferme l'ancien pipe read end
            if (prev_fd != -1)
            {
                close(prev_fd);
            }
            // Ferme cote ecriture du pipe
            if (curr->next)
            {
                close(pipefd[1]);    // Écriture plus nécessaire
                prev_fd = pipefd[0]; // Garder le read end pour la prochaine itération
            }
            else
            {
                // Dernière commande, plus de pipe à garder
                if (pipefd[0] != -1)
                {
                    close(pipefd[0]);
                }
                prev_fd = -1;
            }
        }
        curr = curr->next;
    }
    wait_children();
}

/*
void exec_pipes(t_com_list *cmds, char **envcp)
{
    int prev_fd = -1; // Pour l'entrée (stdin) du processus courant
    int pipefd[2];    // [0] = read end, [1] = write end
    pid_t pid;
    t_com_list *curr = cmds;

    while (curr)
    {
        // Si une commande suit, créer un pipe
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
        if (pid == 0) // === CHILD ===
        {
            printf("Child process %d: %s\n", getpid(), curr->command);
            // Si ce n'est pas la première commande, rediriger stdin depuis le pipe précédent
            if (prev_fd != -1)
            {
                printf("Child %d: Duplicating stdin from fd %d\n", getpid(), prev_fd);
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            // Si ce n'est pas la dernière commande, rediriger stdout vers le pipe actuel
            if (curr->next)
            {
                printf("Child %d: Duplicating stdout to fd %d\n", getpid(), pipefd[1]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]); // Fermeture côté lecture inutile
            }
            char **args = split_args(curr->command, ' ');
            if (!args || !args[0])
            {
                printf("Invalid or empty command: %s\n", curr->command);
                exit(1);
            }
            expand_variables(args, envcp);
            if (is_builting(args[0]))
                exec_builting(args, &envcp);
            else
                exec_cmd(curr, envcp);

            free_tab(args);
            exit(g_exit_status);
        }
        // === PARENT ===
        // Fermer l'entrée précédente (devenue inutile après fork)
        if (prev_fd != -1)
        {
            printf("Parent: Closing fd %d (prev_fd)\n", prev_fd);
            close(prev_fd);
        }

        // Fermer le write-end du pipe (toujours dans le parent)
        if (curr->next)
        {
            printf("Parent: Closing fd %d (pipe write)\n", pipefd[1]);
            close(pipefd[1]);
            prev_fd = pipefd[0]; // à garder pour la prochaine commande
        }
        else
        {
            // Plus de commande suivante : fermeture du read-end aussi
            if (pipefd[0] != -1)
            {
                printf("Parent: Closing final pipe read fd %d\n", pipefd[0]);
                close(pipefd[0]);
            }
            prev_fd = -1;
        }

        curr = curr->next;
    }
    wait_children();
}*/
