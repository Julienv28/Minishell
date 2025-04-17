/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pique <pique@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:01 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/17 14:09:53 by pique            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_echo(char *str, char **envp)
{
	(void)**envp;

	ft_putstr_fd(str, 1);  // La chaîne est déjà traitée
}

char	*get_env_value(char *name, char **envp)
{
	int i;
    size_t len;

	i = 0;
	len = ft_strlen(name);
	if (!name || !envp)
		return (NULL);
    while (envp[i])
    {
		if (envp[i] && ft_strncmp(envp[i], name, len) == 0
			&& envp[i][len] == '=')
			return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

/*
static int	ft_isupper(int c)
{
	if (c >= 65 && c <= 90)
		return (1);
	else
		return (0);
}

void ft_echo(char *str, char **envp)
{
    int i = 0;

	if (!str || str[0] == '\0')
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (str[i] == '\'' || str[i] == '\"')
		i++;
    while (str[i])
	{
        if ((str[i] == '$' && str[0] != '\"') || (str[i] == '$' && str[0] == '\''))
		{
            i++;
            int start = i;
            while (str[i] && (ft_isupper(str[i]) || str[i] == '_'))
                i++;
            int len = i - start;
            char *var = malloc(len + 1);
			if (!var)
				return ;
            for (int j = 0; j < len; j++)
                var[j] = str[start + j];
			var[len] = '\0';

            char *val = get_env_value(var, envp);
            if (val)
                ft_putstr_fd(val, 1);
            free(var);
		}
		else if ((str[i] == '\'' && str[0] != '\"') || (str[0] == '\"'
			&& ((str[i] == '\'' || str[i] == '\"') && str[i + 1] == '\0')))
			i++;
		else
		{
			ft_putchar_fd(str[i], 1);
			i++;
		}
	}
}*/