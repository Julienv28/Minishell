/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:02:35 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/07 08:33:16 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void child_proc(t_com_list *command, char **args, char **envcp, int *fd)
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    if (args && is_builting(args[0]) == 0)
        exec_builting(args, &envcp);
    else
        exec_cmd(command, envcp);
}

static void parent_proc(t_com_list *command, char **args, char **envcp, int *fd)
{
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    close(fd[0]);
    if (args && is_builting(args[0]) == 0 && ft_strcmp(args[0], "cd") != 0)
        exec_builting(args, &envcp);
    else if (ft_strcmp(args[0], "cd") == 0)
        ;
    else
        exec_cmd(command, envcp);
}

void split_args_for_pipe(char **args, char ***left, char ***right)
{
    int i;
    int left_len = 0;
    int right_len = 0;

    while (args[left_len] && ft_strcmp(args[left_len], "|") != 0)
        left_len++;
    if (!args[left_len])
        return;
    i = left_len + 1;
    while (args[i++])
        right_len++;
    *left = malloc(sizeof(char *) * (left_len + 1));
    *right = malloc(sizeof(char *) * (right_len + 1));
    i = 0;
    while (i < left_len)
    {
        (*left)[i] = strdup(args[i]);
        i++;
    }
    (*left)[i] = NULL;
    i = 0;
    while (args[left_len + 1 + i])
    {
        (*right)[i] = strdup(args[left_len + 1 + i]);
        i++;
    }
    (*right)[i] = NULL;
}

void pipex_simple(t_com_list *command, char **args, char **envcp)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;
    char **left_args;
    char **right_args;

    split_args_for_pipe(args, &left_args, &right_args);
    if (pipe(fd) == -1)
        exit_error();
    pid1 = fork();
    if (pid1 == -1)
        exit_error();
    if (pid1 == 0)
        child_proc(command, left_args, envcp, fd);
    pid2 = fork();
    if (pid2 == -1)
        exit_error();
    if (pid2 == 0)
    {
        parent_proc(command, right_args, envcp, fd);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
