#include "../includes/minishell.h"

int	count_ags(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void exec_builting(char **args, char **envp)
{
    int i;

    if (ft_strncmp(args[0], "exit", 5) == 0)
        ft_exit(args);
    else if (ft_strncmp(args[0], "cd", 2) == 0)
        ft_cd(args);
    else if (ft_strncmp(args[0], "pwd", 3) == 0)
        ft_pwd();
    else if (ft_strncmp(args[0], "echo", 4) == 0)
    {
        i = 1;
        if (count_ags(args) == 2)
        	ft_echo(args[1], envp);
		else if (count_ags(args) > 2)
		{
			while (args[i])
			{
				ft_echo(args[i], envp);
				ft_putchar_fd(' ', 1);
				i++;
			}
		}
		ft_putchar_fd('\n', 1);
    }
    else if (ft_strncmp(args[0], "export", 5) == 0)
        ft_export(args, envp);
    //  else if (ft_strncmp(cmd, "unset", 5) == 0)
    //      return (0);
}

char *search_path(char **paths, char *cmd)
{
    int i;
    char *path;
    char *part_path;

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

int find_line(char **envp, char *path)
{
    int i;

    i = 0;
    while (envp[i] && ft_strncmp(path, envp[i], ft_strlen(path)) != 0)
        i++;
    return (i);
}

char *get_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    int line;

    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));
    line = find_line(envp, "PATH");
    if (!envp[line])
        return (NULL);
    paths = ft_split(envp[line] + 5, ':');
    path = search_path(paths, cmd);
    if (path == 0)
    {
        write(2, "command not found: ", 19);
        write(2, cmd, ft_strlen(cmd));
        write(2, "\n", 1);
        exit(0);
    }
    free_tab(paths);
    return (path);
}

void exec_cmd(t_com_list *command, char **envp)
{
    pid_t pid;
    char **args;
    char *path;

    args = ft_split(command->command, ' ');
    path = get_path(args[0], envp);
    if (path == NULL)
    {
        ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
        ft_putstr_fd(args[0], STDERR_FILENO);
        write(2, "\n", 1);
        free_tab(args);
        return;
    }
    pid = fork();
    if (pid == 0) // Si on est dans le processus enfant
    {
        if (execve(path, args, envp) == -1)
        {
            perror("execve failed");
            exit(1);
        }
    }
    else if (pid > 0)          // Si on est dans le processus parent
        waitpid(pid, NULL, 0); // Attente de la fin de l'exécution du processus enfant
    else                       // Gestion d'une erreur de fork
        perror("fork failed");
    free(path);
}