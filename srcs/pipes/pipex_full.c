/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_full.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:51:20 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/15 17:23:06 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
static void child_process(t_com_list *command, char *arg, char **envcp)
{
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1)
        exit_error();
    pid = fork();
    if (pid == -1)
        exit_error();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        if (arg && is_builting(arg) == 0)
            exec_builting(&arg, &envcp);
        else
            exec_cmd(command, &envcp);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        waitpid(pid, NULL, 0);
    }
}

static void here_doc_handler(char *limiter, int ac)
{
    pid_t reader;
    int fd[2];
    char *line;

    if (ac < 6)
        exit_error();
    if (pipe(fd) == -1)
        exit_error();
    reader = fork();
    if (reader == 0)
    {
        close(fd[0]);
        while (simplified_gnl(&line))
        {
            if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
                exit(EXIT_SUCCESS);
            write(fd[1], line, ft_strlen(line));
        }
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        wait(NULL);
    }
}

void complex_pipex(t_com_list *command, int ac, char **args, char **envcp)
{
    int i;
    int filein;
    int fileout;

    if (ac >= 5)
    {
        if (ft_strncmp(args[1], "here_doc", 8) == 0)
        {
            i = 3;
            fileout = open_file(args[ac - 1], 0);
            here_doc_handler(args[2], ac);
        }
        else
        {
            i = 2;
            fileout = open_file(args[ac - 1], 1);
            filein = open_file(args[1], 2);
            dup2(filein, STDIN_FILENO);
        }
        while (i < ac - 2)
            child_process(command, args[i++], envcp);
        dup2(fileout, STDOUT_FILENO);
        if (args && is_builting(args[0]) == 0)
            exec_builting(args, &envcp);
        else
            exec_cmd(command, &envcp);
    }
    exit_error();
}*/
