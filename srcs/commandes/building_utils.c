/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   building_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:32:08 by opique            #+#    #+#             */
/*   Updated: 2025/06/11 16:56:50 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_char_index(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	check_events(char *arg)
{
	char	*res;
	int		index;

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

void	cleanup_and_exit(int code, t_com *cmd)
{
	free_cmd(cmd);
	rl_clear_history();
	exit(code);
}

char	**ft_env_dup(char **envp)
{
	int		i;
	char	**envcp;

	i = 0;
	while (envp[i])
		i++;
	envcp = malloc(sizeof(char *) * (i + 1));
	if (!envcp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envcp[i] = ft_strdup(envp[i]);
		if (!envcp[i])
		{
			while (--i >= 0)
				free(envcp[i]);
			free(envcp);
			return (NULL);
		}
		i++;
	}
	envcp[i] = NULL;
	return (envcp);
}
