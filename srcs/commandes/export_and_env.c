#include "../includes/minishell.h"

// Fonction pour vérifier la validité du nom de la variable
int is_valid_name(char *name)
{
    int i;

    if (!name || (!ft_isalpha(name[0]) && name[0] != '_')) // Le nom doit commencer par une lettre ou un underscore
        return (0);
    i = 1;
    while (name[i])
    {
        if (!(ft_isalnum(name[i]) || name[i] == '_')) // Le reste doit être alphanumérique ou underscore
        {
            printf("restant lettres fausses\n");
            return (0);
        }
        i++;
    }
    return (1);
}

void	ft_set_env(char *key, char *value, char ***envp)
{
	int		i;
	size_t	len;
	char	*new_entry;

	i = 0;
	len = ft_strlen(key);
	new_entry = ft_srjoin3(key, "=", value);
	//if (!key || !value || !envp)
	//	return;
	
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, len) == 0 && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return ;
		}
		i++;
	}
	*envp = ft_realloc_env(*envp, new_entry);
}

void	ft_export(char *arg, char ***envp)
{
	char	*key;
	char	*value;

	if (!arg || !ft_strchr(arg, '='))
		return ;
	key = ft_substr((const char *)arg, 0, ft_strchr(arg, '=') - arg);
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	if (is_valid_name(key))
		ft_set_env(key, value, envp);
	free(key);
	free(value);
}

void	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
