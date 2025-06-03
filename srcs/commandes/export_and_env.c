/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/03 17:04:45 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Fonction pour vérifier la validité du nom de la variable
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

void	ft_set_env(char *key, char *value, char ***envcp)
{
	int		i;
	char	*new_entry;
	char	*tmp;
	char	**new_env;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return ;
	new_entry = ft_strjoin(tmp, value ? value : "");
	free(tmp);
	if (!new_entry)
		return ;
	i = 0;
	while (*envcp && (*envcp)[i])
	{
		if (ft_strncmp((*envcp)[i], key, ft_strlen(key)) == 0
			&& (*envcp)[i][ft_strlen(key)] == '=')
		{
			free((*envcp)[i]);
			(*envcp)[i] = new_entry;
			return ;
		}
		i++;
	}
	new_env = ft_realloc_env(*envcp, new_entry);
	if (!new_env)
	{
		free(new_entry);
		return ;
	}
	ft_freeenvp(*envcp);
	*envcp = new_env;
}

int ft_export(char **args, char ***envcp)
{
    char *key = NULL;
    char *value = NULL;
    char *equal_sign = NULL;
    char *replaced = NULL;
    char *expanded_value;
    char *tmp;
    int exit_status = 0;
    int i = 1;
    int j;

    if (!args[1])
    {
        i = 0;
        while ((*envcp)[i])
        {
            j = 0;
            printf("declare -x ");
            while ((*envcp)[i][j])
            {
                if ((*envcp)[i][j] == '=')
                {
                    printf("=\"%s\"\n", (*envcp)[i] + j + 1);
                    break;
                }
                putchar((*envcp)[i][j]);
                j++;
            }
            if (!ft_strchr((*envcp)[i], '='))
                printf("\n");
            i++;
        }
        return (0);
    }

    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            int key_len = equal_sign - args[i];
            key = ft_substr(args[i], 0, key_len);
            value = ft_strdup(equal_sign + 1);
            expanded_value = replace_all_variables(value, *envcp, 0);
            free(value);
            value = expanded_value ? expanded_value : ft_strdup("");
            replaced = ft_strjoin(key, "=");
            tmp = ft_strjoin(replaced, value);
            free(replaced);
            replaced = tmp;
        }
        else
            replaced = ft_strdup(args[i]);
        if (!replaced || replaced[0] == '\0')
        {
            ft_putstr_fd("export: `': not a valid identifier\n", STDERR_FILENO);
            free(replaced);
            exit_status = 1;
            i++;
            continue;
        }
        if (replaced[0] == '-')
        {
            printf("bash: export: -%c: invalid option\n", replaced[1]);
            free(replaced);
            exit_status = 2;
            i++;
            continue;
        }
        char *pos_equal = ft_strchr(replaced, '=');
        if (pos_equal)
        {
            free(key);
            key = ft_substr(replaced, 0, pos_equal - replaced);
        }
        else
        {
            free(key);
            key = ft_strdup(replaced);
        }
        if (!is_valid_name(key))
        {
            printf("bash: export: %s: not a valid identifier\n", replaced);
            exit_status = 1;
            free(key);
            free(replaced);
            free(value);
            i++;
            continue;
        }
        if (equal_sign)
            ft_set_env(key, value, envcp);
        free(key);
        free(value);
        free(replaced);
        i++;
    }
    return (exit_status);
}

void ft_env(char **envcp)
{
    int i;

    i = 0;
    while (envcp && envcp[i])
    {
        printf("%s\n", envcp[i]);
        i++;
    }
}

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
