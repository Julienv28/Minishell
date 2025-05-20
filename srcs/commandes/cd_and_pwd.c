#include "../includes/minishell.h"

void ft_cd(char **args, char ***envcp)
{
    char current_dir[1024];
    char new_dir[1024];
    char *path;

    if (!getcwd(current_dir, sizeof(current_dir)))
    {
        ft_putstr_fd("chdir: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
        perror("");
        chdir("/");
        return;
    }

    // Trop d’arguments
    if (args[1] && args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return;
    }

    // Cas : cd ou cd --
    if (!args[1] || ft_strcmp(args[1], "--") == 0)
    {
        path = get_env_value("HOME", *envcp);
        if (!path)
        {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            return;
        }
    }
    // Cas : cd -
    else if (ft_strcmp(args[1], "-") == 0)
    {
        path = get_env_value("OLDPWD", *envcp);
        if (!path)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
            return;
        }
        printf("%s\n", path); // Affiche le répertoire comme bash
    }
    // Cas option invalide
    else if (args[1][0] == '-' && ft_strcmp(args[1], "--") != 0)
    {
        ft_putstr_fd("minishell: cd: invalid option\n", STDERR_FILENO);
        ft_putstr_fd("cd: usage: cd [dir]\n", STDERR_FILENO);
        return;
    }
    // Cas normal : cd dossier
    else
        path = args[1];
    // Vérifie si le chemin existe
    if (access(path, F_OK) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        return;
    }

    // Vérifie si c'est bien un répertoire
    if (!is_directory(path))
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
        return;
    }

    // Vérifie si le chemin est accessible (exécutable)
    if (access(path, X_OK) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        return;
    }

    // Change de dossier
    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
        return;
    }

    // Récupère le nouveau répertoire
    if (!getcwd(new_dir, sizeof(new_dir)))
    {
        perror("cd: getcwd (after chdir) failed");
        return;
    }

    // Met à jour OLDPWD et PWD
    ft_set_env("OLDPWD", current_dir, envcp);
    ft_set_env("PWD", new_dir, envcp);
}

// PWD
// Affiche le chemin absolu du répertoire courant.

void ft_pwd(char **args)
{
    char path[1024];

    // Verifier la presence d'options
    if (args[1] && args[1][0] == '-' && count_ags(args) == 2)
    {
        printf("bash: pwd: -%c: invalid option\n", args[1][1]);
        return;
    }
    // Récupérer le répertoire courant
    if (getcwd(path, sizeof(path)) != NULL)
        printf("%s\n", path);
    else
    {
        ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
        perror("");
        return;
    }
}