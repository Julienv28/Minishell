/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:30:12 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/02 12:06:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>

int	count_ags(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int exec_builting(char **args, char ***envcp)
{
    if (ft_strcmp(args[0], "exit") == 0)
        return(ft_exit(args, 1), 0); //
    else if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(args, envcp));
    else if (ft_strcmp(args[0], "pwd") == 0)
        return (ft_pwd(args, envcp));
    else if (ft_strcmp(args[0], "echo") == 0)
        return (ft_echo(args, envcp), 0); // Appel direct avec les args
    else if (ft_strcmp(args[0], "export") == 0)
    {
        if (!args[1]) // Aucun argument : afficher l'environnement
            return (ft_env(*envcp), 0);
        else
        {
            if (check_events(args[1]) == 0)
                return (ft_export(args, envcp));
            else
                return (1);
        }
    }
    else if (ft_strcmp(args[0], "env") == 0)
    {
        if (args[1] && args[1][0] == '-')
        {
            printf("bash: env: -%c: invalid option\n", args[1][1]);
            g_exit_status = 2;
            return (g_exit_status);
        }
        else
            return (ft_env(*envcp), 0);
    }
    else if (ft_strcmp(args[0], "unset") == 0)
    {
        if (check_events(args[1]) == 0)
            return (ft_unset(args, envcp));
        else
            return (1);
    }
    return (g_exit_status);
}

int	is_valid_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*search_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*part_path;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (0);
}

int	find_line(char **envp, char *path)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(path, envp[i], ft_strlen(path)) != 0)
		i++;
	return (i);
}

int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}
char *path_error_message(char *cmd)
{
    if (access(cmd, F_OK) != 0)
    {
        printf("minishell: %s : No such file or directory\n", cmd);
        g_exit_status = 127;
        return (NULL);
    }
    if (is_directory(cmd))
    {
        printf("minishell: %s : is a directory\n", cmd);
        g_exit_status = 126;
        return (NULL);
    }
    if (access(cmd, X_OK) != 0)
    {
        printf("minishell: %s : Permission denied\n", cmd);
        g_exit_status = 126;
        return (NULL);
    }
    return (ft_strdup(cmd));
}

char *get_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    int line;

    if (ft_strchr(cmd, '/'))
        return ((path_error_message(cmd)));
    line = find_line(envp, "PATH");
    if (!envp[line] || line == -1)
        return (ft_putstr_fd("minishell: PATH not set\n", STDERR_FILENO), NULL);
    paths = ft_split(envp[line] + 5, ':');
    path = search_path(paths, cmd);
    if (path == NULL)
    {
        fprintf(stderr, "minishell: %s : command not found\n", cmd);
        g_exit_status = 127;
    }
    else if (access(path, X_OK) != 0)
    {
        printf("minishell: %s : Permission denied\n", cmd);
        g_exit_status = 126;
        free(path);
        path = NULL;
    }
    free_tab(paths);
    return (path);
}

void exec_cmd(char **args, char ***envcp)
{
    char *path;

	path = get_path(args[0], *envcp);
	if (path == NULL)
		exit(g_exit_status);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, args, *envcp) == -1)
	{
        if (errno == EISDIR)
        {
            printf("minishell: %s : Is a directory\n", path);
            g_exit_status = 126;
        }
        else
        {
            perror("minishell");
            g_exit_status = 1;
        }
        free(path);
        exit(g_exit_status);
    }
    exit(1);
}
