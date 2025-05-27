/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_and_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:04:31 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/27 17:02:57 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_cd(char **args, char ***envcp)
{
    char current_dir[1024];
    char new_dir[1024];
    char *path = NULL;
    char *home = get_env_value("HOME", *envcp);

    // Tenter de récupérer le répertoire courant AVANT cd
    if (!getcwd(current_dir, sizeof(current_dir)))
        current_dir[0] = '\0'; // Si échec, on laisse vide pour éviter fausse info

    // Trop d'arguments
    if (args[1] && args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    // Cas cd ou cd --
    if (!args[1] || ft_strcmp(args[1], "--") == 0)
    {
        if (!home)
        {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            return (1);
        }
        path = home;
    }
    // Cas cd -
    else if (ft_strcmp(args[1], "-") == 0)
    {
        path = get_env_value("OLDPWD", *envcp);
        if (!path)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        // Affiche le nouveau chemin
        printf("%s\n", path);
    }
    // Cas: option invalide
    else if (args[1][0] == '-' && ft_strcmp(args[1], "-") != 0 && ft_strcmp(args[1], "--") != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(args[1], STDERR_FILENO);
        ft_putstr_fd(": invalid option\n", STDERR_FILENO);
        g_exit_status = 2;
        return (g_exit_status);
    }
    else if (args[1][0] == '~')
    {
        if (!home)
        {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            return (1);
        }
        size_t home_len = strlen(home);
        size_t suffix_len = strlen(args[1] + 1);
        path = malloc(home_len + suffix_len + 1);
        if (!path)
            return (1);
        strcpy(path, home);
        strcat(path, args[1] + 1); // Ajoute le reste du chemin après '~'
    }

    else
        path = args[1];

    // Tenter de changer de répertoire
    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        return (1);
    }

    // Vérifie que le nouveau répertoire est valide
    if (!getcwd(new_dir, sizeof(new_dir)))
    {
        ft_putstr_fd("chdir: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
        perror("");
        return (1);   
    }

    // Mise à jour PW D et OLDPWD
    if (current_dir[0] != '\0')
        ft_set_env("OLDPWD", current_dir, envcp);
    ft_set_env("PWD", new_dir, envcp);

    // Libère si on a alloué pour ~
    if (args[1] && args[1][0] == '~')
        free(path);
    return (0);
}

// PWD
// Affiche le chemin absolu du répertoire courant.
int ft_pwd(char **args, char ***envcp)
{
    (void)args;
    char path[1024];

    // Vérifier la présence d'options
    if (args[1] && args[1][0] == '-' && args[1][1] != '\0')
    {
        printf("minishell: pwd: -%c: invalid option\n", args[1][1]);
        g_exit_status = 2;
        return (g_exit_status);
    }

    // Tenter de récupérer le répertoire courant
    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
        return (0);
    }        
    else
    {
        // Fallback sur la variable d'environnement PWD
        char *pwd = get_env_value("PWD", *envcp);
        if (pwd)
        {
            printf("%s\n", pwd);
            return (0);
        }
        
        ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: ", STDERR_FILENO);
        return (1);
    }
}
