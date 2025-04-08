#include "../includes/minishell.h"

// CD
// change de repertoire courant
// Ne prend qu’un chemin absolu ou relatif (cd .., cd /, cd ./folder, etc.).
// Ne gère pas cd -, cd ~, etc., sauf si bonus.

// Verifier le chemin passée en argument est valide (access)
// si existe pas afficher un message d'erreur.
// Une fois chemin est valide, chdir() pour changer de répertoire.
// Si simple cd récupérer la valeur de HOME et l’utiliser pour faire le changement de répertoire.

void ft_cd(char **args)
{
    char current_dir[1024];
    char *path;
    char *current_pwd;

    current_pwd = NULL;
    // Sauvegarder l'ancien répertoire
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("getcwd failed");
        return;
    }
    // Si l'argument est vide, rediriger vers HOME
    if (args[1] == NULL)
    {
        path = getenv("HOME");
        if (path == NULL)
        {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            return;
        }
    }
    // Sinon, l'argument est un chemin donné par l'utilisateur
    else
        path = args[1];
    if (access(path, F_OK) == -1)
    {
        ft_putstr_fd("minishell: cd: no such file or directory: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        return;
    }
    // Changer de repertoire
    if (chdir(path) == -1)
    {
        perror("cd failed");
        return;
    }
    // Mettre à jour PWD
    if (current_pwd != NULL)
        free(current_pwd);
    current_pwd = ft_strdup(path);
    printf("repertoire actuel: %s\n", path);
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
        perror("getcwd");
}