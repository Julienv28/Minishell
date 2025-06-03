/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/03 13:43:25 by juvitry          ###   ########.fr       */
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
    char *key;
    char *value;
    char *equal_sign;
    char *replaced;
    int exit_status = 0;
    int i;
    int j;

    i = 0;
    if (!args[1])
    {
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
    i = 1;
    while (args[i])
    {
        replaced = replace_all_variables(args[i], *envcp, 1);
        if (!replaced)
        {
            i++;
            continue;
        }
        if (replaced[0] == '\0')
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
        equal_sign = ft_strchr(replaced, '=');
        if (equal_sign)
        {
            key = ft_substr(replaced, 0, equal_sign - replaced);
            value = ft_strdup(equal_sign + 1);
        }
        else
        {
            key = ft_strdup(replaced);
            value = NULL;
        }
        if (!is_valid_name(key))
        {
            printf("bash: export: %s: not a valid identifier\n", replaced);
            exit_status = 1;
            free(key);
            free(replaced);
            i++;
            continue;
        }
        if (equal_sign)
        {
            // printf("Ajout ou modification de l'environnement : %s=%s\n", key, value);
            ft_set_env(key, value, envcp);
        }
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
            return i;
        i++;
    }
    return -1;
}

int check_events(char *arg)
{
    char *res;
    int index;

    index = find_char_index(arg, '!');
    if (index >= 0)
    {
        res = ft_substr(arg, index, ft_strlen(arg) - index);
        ft_putstr_fd("Minishell: ", STDERR_FILENO);
        ft_putstr_fd(res, STDERR_FILENO);
        ft_putstr_fd(": event not found\n", STDERR_FILENO);
        free(res);
        return (1);
    }
    return (0);
}
