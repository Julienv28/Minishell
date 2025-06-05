#include "../includes/minishell.h"

void	free_export_vars(char *key, char *value, char *replaced)
{
    free(key);
    free(value);
    free(replaced);
}

// Free environnement
void	ft_freeenvp(char **envcp)
{
	int	i;

	i = 0;
	if (!envcp)
		return ;
	while (envcp[i])
	{
		free(envcp[i]);
		i++;
	}
	free(envcp);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char *free_all(char *before, char *var_key, char *spaced, char *value)
{
	if (before)
		free(before);
	if (var_key)
		free(var_key);
	if (spaced)
		free(spaced);
	if (value)
		free(value);
	return (NULL);
}