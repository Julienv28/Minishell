#include "../includes/minishell.h"

int count_ags(char **args)
{
    int i;

    i = 0;
    while (args[i])
        i++;
    return (i);
}

void exec_builting(char **args, char ***envcp)
{
    int i;

    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        ft_cd(args, envcp);
    else if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd(args);
    else if (ft_strcmp(args[0], "echo") == 0)
    {
        if (parse_args_echo(args) == 1)
        {
            return;
        }
        if (ft_strcmp(args[1], "-n") != 0)
        {
            i = 1;
            if (count_ags(args) == 2)
                ft_echo(args[1], *envcp);
            else if (count_ags(args) > 2)
            {
                while (args[i])
                {
                    ft_echo(args[i], *envcp);
                    if (args[i + 1] != NULL)
                        ft_putchar_fd(' ', 1);
                    i++;
                }
            }
            ft_putchar_fd('\n', 1);
        }
        else
        {
            i = 2;
            if (count_ags(args) == 3)
                ft_echo(args[2], *envcp);
            else if (count_ags(args) > 3)
            {
                while (args[i])
                {
                    ft_echo(args[i], *envcp);
                    if (args[i + 1] != NULL)
                        ft_putchar_fd(' ', 1);
                    i++;
                }
            }
        }
    }
    else if (ft_strcmp(args[0], "export") == 0)
    {
        // if (count_ags(args) > 2)
        // {
        //     printf("minishell: export: `%s': not a valid identifier\n", args[2]);
        //     return;
        // }
        if (!args[1]) // Aucun argument : afficher l'environnement
        {
            printf("minishell: export: `%s': not a valid identifier\n", args[2]);
            return;
        }
        else if (check_events(args[1]) == 0)
            ft_export(args, envcp);
        else
            return;
    }
    else if (ft_strcmp(args[0], "env") == 0)
        ft_env(*envcp);
    else if (ft_strcmp(args[0], "unset") == 0)
        ft_unset(args, envcp);
    else
        return;
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

// determine si un chemin passé a path est un repertoire ou non
//  S_ISDIR = Macro pour savoir si c'est un repertoire
int is_directory(char *path)
{
    struct stat sb;                                  // utilisation structure stat pour stocker les infos d'un repertoire
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) // Passer le chemin et un ptr ver la structure pour remplir les infos
        return (1);                                  // Si chemin trouver c'est un repertoire
    return (0);
}

char *get_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    int line;

    // Si la commande est déjà un chemin absolu et exécutable
    if (access(cmd, F_OK) == 0) // On vérifie d'abord si le fichier existe
    {
        if (is_directory(cmd)) // Vérification si c'est un répertoire
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": is a directory\n", STDERR_FILENO);
            return (NULL);
        }
        if (access(cmd, X_OK) == 0)  // Vérification si c'est exécutable
            return (ft_strdup(cmd)); // Commande trouvée et exécutable
        else
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            return (NULL); // Commande trouvée mais pas exécutable
        }
    }

    // Recherche de la commande dans les chemins spécifiés dans PATH
    line = find_line(envp, "PATH");
    if (!envp[line]) // Si le PATH n'est pas trouvé
        return (NULL);

    // Découper le chemin du PATH en différents répertoires
    paths = ft_split(envp[line] + 5, ':');
    // Recherche dans les répertoires du PATH
    path = search_path(paths, cmd);

    if (path == NULL) // Commande non trouvée dans les chemins de PATH
    {
        ft_putstr_fd("minishell: command not found: ", STDERR_FILENO); // Commande introuvable
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
    }
    else if (access(path, X_OK) != 0) // Vérification si le fichier est exécutable
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        free(path);  // Libération de la mémoire allouée pour path
        path = NULL; // Remise à NULL pour indiquer l'échec
    }

    free_tab(paths); // Libération de la mémoire allouée pour les chemins
    return (path);   // Retourne NULL si la commande n'est pas trouvée ou pas exécutable
}

void exec_cmd(t_com_list *command, char **envcp)
{
    pid_t pid;
    char **args;
    char *path;
    int status;

    g_exit_status = 1;
    args = ft_split(command->command, ' ');
    if (!args || args[0] == NULL || args[0][0] == '\0')
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(*args, STDERR_FILENO);
        ft_putstr_fd("command not found\n", STDERR_FILENO);
        g_exit_status = 127;
        free_tab(args);
        return;
    }
    path = get_path(args[0], envcp);
    if (path == NULL)
    {
        g_exit_status = 127; // Commande introuvable
        free_tab(args);
        return;
    }
    pid = fork();
    if (pid == 0) // Si on est dans le processus enfant
    {
        if (execve(path, args, envcp) == -1)
        {
            perror("execve failed");
            exit(1);
        }
    }
    else if (pid > 0) // Si on est dans le processus parent
    {
        waitpid(pid, &status, 0); // Attente de la fin de l'exécution du processus enfant
        // MAJ g_exit_status pour avoir la bonne valeur
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status); // Code de sortie normal
        else if (WIFSIGNALED(status))
            g_exit_status = 128 + WTERMSIG(status); // Interrompu par signal
    }
    else
    { // Gestion d'une erreur de fork
        perror("fork failed");
        g_exit_status = 1;
    }
    free(path);
    free_tab(args);
}
