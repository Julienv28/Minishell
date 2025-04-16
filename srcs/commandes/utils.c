/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/16 15:07:07 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Liberer tab args
void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
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

void	ft_freeenvp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free (envp);
}

char	*ft_srjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;

	//if (!s1 || !s2 || !s3)
	//	return (NULL);
	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	**ft_realloc_env(char **envp, char *new_entry)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (envp && envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_env[j] = ft_strdup(envp[j]);
		j++;
	}
	new_env[i] = ft_strdup(new_entry);
	new_env[i + 1] = NULL;
	free_tab(envp);
	return (new_env);
}
