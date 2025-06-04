#include "../includes/minishell.h"

// EXPORT ET UNSET
static int find_char_index(const char *s, char c)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return (i);
        i++;
    }
    return (-1);
}
// EXPORT 
int check_events(char *arg)
{
    char *res;
    int index;

    index = find_char_index(arg, '!');
    if (index >= 0)
    {
        res = ft_substr(arg, index, ft_strlen(arg) - index);
        printf("Minishell: %s: event not found\n", res);
        free(res);
        return (1);
    }
    return (0);
}
// Le nom doit commencer par une lettre ou un underscore
// Le reste doit être alphanumérique ou underscore
int	is_valid_name(char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}


char    **ft_realloc_env(char **envcp, char *new_entry)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (envcp && envcp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_env[j] = ft_strdup(envcp[j]);
		if (!new_env[j])
		{
			while (--j >= 0)
				free(new_env[j]);
			free(new_env);
			return (NULL);
		}
		j++;
	}
	new_env[i] = new_entry;
	if (!new_env[i])
	{
		while (--j >= 0)
			free(new_env[j]);
		free(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	return (new_env);
}