/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/20 17:34:53 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>


void execute(t_com_list *cmds, char ***envcp)
{
    char **args;
    pid_t pid;

    if (!cmds)
        return;
    args = split_args(cmds->command, ' ');
    if (!args || !args[0])
    {
        free_tab(args);
        return;
    }

    expand_variables(args, *envcp);
    if (args[0] == NULL || args[0][0] == '\0')
    {
        fprintf(stderr, "minishell: command not found\n");
        free_tab(args);
        return;
    }
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
            exec_cmd(args, envcp);
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
/*
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

                if (curr->outfile)
                {
                    int fd_out = open_outfile(curr->outfile, curr->flag_out);
                    if (fd_out < 0)
                    {
                        perror(curr->outfile);
                        exit(1);
                    }
                    dup2(fd_out, STDOUT_FILENO); // Rediriger stdout vers fichier
                    close(fd_out);
                }
            }
            // Prépare les arguments
            char **args = split_args(curr->command, ' ');
            if (!args || !args[0])
                exit(1);

            expand_variables(args, *envcp);

            if (is_builting(args[0]))
                exec_builting(args, envcp); // forké même si builtin
            else
                exec_cmd(args, envcp);

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
}*/

void exec_pipes(t_com_list *cmds, char ***envcp)
{
    int pipefd[2];
    int prev_fd = -1; // read end du pipe précédent
    pid_t pid;
    t_com_list *curr = cmds;

    while (curr)
    {
        // Création d’un nouveau pipe si une commande suivante existe
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
            // Redirection de l'entrée depuis le pipe précédent
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // Redirection de la sortie vers le pipe suivant
            if (curr->next)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }
            else
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Redirections fichiers (même dans les pipes !)
            if (curr->infile || curr->outfile || curr->errfile || curr->all_outfilles)
            {
                if (ft_redirection(curr, NULL, NULL, NULL) < 0)
                {
                    perror("redirection");
                    exit(1);
                }
            }

            // Préparation et exécution de la commande
            char **args = split_args(curr->command, ' ');
            if (!args || !args[0])
                exit(1);

            expand_variables(args, *envcp);

            if (is_builting(args[0]))
                exec_builting(args, envcp); // builtin dans le child
            else
                exec_cmd(args, envcp);

            free_tab(args);
            exit(g_exit_status);
        }
        else // === PARENT ===
        {
            // Fermer le read end précédent s’il existe
            if (prev_fd != -1)
                close(prev_fd);

            // Préparer le read end pour la prochaine commande
            if (curr->next)
            {
                close(pipefd[1]);        // Écriture plus nécessaire
                prev_fd = pipefd[0];     // Lecture pour le prochain enfant
            }
            else
            {
                close(pipefd[0]);
                close(pipefd[1]);
                prev_fd = -1;
            }
        }
        curr = curr->next;
    }

    wait_children();
}


