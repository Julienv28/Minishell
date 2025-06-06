#include "../../includes/minishell.h"

void	copy_cleaned(const char *str, char *res)
{
	int	i;
	int	j;
	int	in_space;

	i = 0;
	j = 0;
	in_space = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			if (!in_space)
				res[j++] = ' ';
			in_space = 1;
		}
		else
		{
			res[j++] = str[i];
			in_space = 0;
		}
		i++;
	}
	if (j > 0 && res[j - 1] == ' ')
		j--;
	res[j] = '\0';
}

char	*remove_extra_spaces(const char *str)
{
	char	*res;

	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	copy_cleaned(str, res);
	return (res);
}

char	*clean_spaces(char *str)
{
	int		i;
	char	*trimmed;
	char	*result;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	trimmed = ft_strdup(str + i);
	if (!trimmed)
		return (NULL);
	result = remove_extra_spaces(trimmed);
	free(trimmed);
	return (result);
}

char	*get_value_cleaned(char *name, char **envp)
{
	char	*raw;
	char	*value;

	raw = get_env_value(name, envp);
	if (!raw)
		return (ft_strdup(""));
    value = clean_spaces(raw);
	printf("[DEBUG] get_value_cleaned: returning '%s'\n", value);
    if (!value)
	{
		printf("[DEBUG] get_value_cleaned: clean_spaces failed\n");
		return (ft_strdup(""));
	}
	printf("[DEBUG] get_value_cleaned: returning '%s'\n", value);
	return (value);
}
