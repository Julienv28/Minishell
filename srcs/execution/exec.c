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
    // int i;

    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit(args);
    else if (ft_strcmp(args[0], "cd") == 0)
        ft_cd(args, envcp);
    else if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd(args);
    else if (ft_strcmp(args[0], "echo") == 0)
    {
        ft_echo(args); // Appel direct avec les args
    }
    else if (ft_strcmp(args[0], "export") == 0)
    {
        if (!args[1]) // Aucun argument : afficher l'environnement
        {
            ft_env(*envcp); // Affiche les variables d'environnement
            return;
        }
        else
        {
            if (check_events(args[1]) == 0)
                ft_export(args, envcp);
            else
                return;
        }
    }
    else if (ft_strcmp(args[0], "env") == 0)
    {
        // Verifier la presence d'options
        if (args[1] && args[1][0] == '-')
        {
            printf("bash: env: -%c: invalid option\n", args[1][1]);
            return;
        }
        else
            ft_env(*envcp);
    }
    else if (ft_strcmp(args[0], "unset") == 0)
        ft_unset(args, envcp);
    else
        return;
}

int is_valid_n_flag(const char *str)
{
    int i = 1;

    if (str[0] != '-' || str[1] != 'n')
        return 0;

    while (str[i])
    {
        if (str[i] != 'n')
            return 0;
        i++;
    }
    return 1;
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
/*
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
}*/

char *get_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    int line;

    // Cas : la commande contient un '/' => chemin absolu ou relatif
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            return (NULL);
        }
        if (is_directory(cmd))
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": is a directory\n", STDERR_FILENO);
            return (NULL);
        }
        if (access(cmd, X_OK) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            return (NULL);
        }
        return (ft_strdup(cmd));
    }

    // Sinon : on cherche dans le PATH
    line = find_line(envp, "PATH");
    if (!envp[line])
        return (NULL);

    paths = ft_split(envp[line] + 5, ':');
    path = search_path(paths, cmd);

    if (path == NULL)
    {
        ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
    }
    else if (access(path, X_OK) != 0)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        free(path);
        path = NULL;
    }

    free_tab(paths);
    return (path);
}


void exec_cmd(char **args, char ***envcp)
{
    char *path;

    // if (!args || args[0] == NULL || args[0][0] == '\0')
    // {
    //     ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
    //     ft_putstr_fd("(null)\n", STDERR_FILENO);
    //     exit(127);
    // }
    path = get_path(args[0], *envcp);

    //printf("path = %s\n", path);
    if (path == NULL)
    {
        // ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
        // ft_putstr_fd(args[0], STDERR_FILENO);
        // ft_putstr_fd("\n", STDERR_FILENO);
        g_exit_status = 127;
        exit(127);
    }
    // Dans exec_cmd
    if (execve(path, args, *envcp) == -1)
    {
        // if (errno == ENOENT)
        //     fprintf(stderr, "minishell: %s: No such file or directory\n", path);
        // else if (errno == EACCES)
        //     fprintf(stderr, "minishell: %s: Permission denied\n", path);
        // else
        //     perror("minishell");
        perror("minishell");
        free(path);
        exit(127);
    }

    execve(path, args, *envcp);
    perror("execve failed");
    free(path);
    exit(1);
}
