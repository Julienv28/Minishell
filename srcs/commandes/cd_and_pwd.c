#include "../includes/minishell.h"

void ft_cd(char **args, char ***envp)
{
    char current_dir[1024];
    char new_dir[1024];
    char *path;

    // Vérifie le répertoire courant actuel
    if (!getcwd(current_dir, sizeof(current_dir)))
    {
        perror("cd: getcwd failed");
        return;
    }

    // Affiche la valeur actuelle de OLDPWD avant de le mettre à jour
    char *oldpwd_before = get_env_value("OLDPWD", *envp);
    if (oldpwd_before)
        printf("OLDPWD avant mise à jour: %s\n", oldpwd_before);
    else
        printf("OLDPWD avant mise à jour: NULL\n");

    // Gestion des arguments
    if (args[1] && args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return;
    }

    // Cas de cd sans argument, ou cd --
    if (!args[1] || ft_strcmp(args[1], "--") == 0)
    {
        path = get_env_value("HOME", *envp);
        if (!path)
        {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            return;
        }
    }
    // Cas de cd - (pour revenir au répertoire précédent)
    else if (ft_strcmp(args[1], "-") == 0)
    {
        // Récupère la valeur de OLDPWD
        path = get_env_value("OLDPWD", *envp);

        if (!path)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
            return;
        }

        // Affiche le chemin complet de OLDPWD pour déboguer
        printf("Chemin OLDPWD avant chdir: %s\n", path);

        // Vérifie si le répertoire OLDPWD existe avant de changer
        if (access(path, F_OK) == -1)
        {
            ft_putstr_fd("cd: no such file or directory: ", STDERR_FILENO);
            ft_putstr_fd(path, STDERR_FILENO);
            ft_putstr_fd("\n", STDERR_FILENO);
            return;
        }

        // Affiche le répertoire de retour pour déboguer
        printf("Reverting to OLDPWD: %s\n", path);
    }
    // Cas d'un répertoire spécifié
    else
        path = args[1];

    // Vérifie si le dossier spécifié existe
    if (access(path, F_OK) == -1)
    {
        ft_putstr_fd("minishell: cd: no such file or directory: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        return;
    }

    // Avant de changer de répertoire, mets à jour OLDPWD avec le répertoire actuel
    ft_set_env("OLDPWD", current_dir, envp);

    // Change de répertoire
    if (chdir(path) == -1)
    {
        perror("cd failed");
        return;
    }

    // Récupère le nouveau répertoire courant
    if (!getcwd(new_dir, sizeof(new_dir)))
    {
        perror("cd: getcwd (after chdir) failed");
        return;
    }

    // Met à jour PWD avec le nouveau répertoire
    ft_set_env("PWD", new_dir, envp);

    // Affiche les valeurs des variables d'environnement après la mise à jour
    printf("PWD après mise à jour: %s\n", new_dir);
    printf("OLDPWD après mise à jour: %s\n", get_env_value("OLDPWD", *envp));
}


// PWD
// Affiche le chemin absolu du répertoire courant.

void ft_pwd(void)
{
    char path[1024];

    // Récupérer le répertoire courant
    if (getcwd(path, sizeof(path)) != NULL)
        printf("Répertoire actuel : %s\n", path);
    else
        perror("getcwd\n");
}