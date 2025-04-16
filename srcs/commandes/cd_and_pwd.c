#include "../includes/minishell.h"

#include "../includes/minishell.h"

void	ft_cd(char **args, char ***envcp)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*path;

	if (!getcwd(current_dir, sizeof(current_dir)))
	{
		perror("cd: getcwd failed");
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

	// Vérifie que le chemin est accessible
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory or permission denied\n", STDERR_FILENO);
		return;
	}

	// Change de dossier
	if (chdir(path) == -1)
	{
		perror("cd failed");
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

void ft_pwd(void)
{
    char path[1024];

    // Récupérer le répertoire courant
    if (getcwd(path, sizeof(path)) != NULL)
        printf("Répertoire actuel : %s\n", path);
    else
        perror("getcwd\n");
}