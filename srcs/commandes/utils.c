/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pique <pique@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 18:10:28 by pique            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	free_tab(char **tab)
// {
// 	int	i;

// 	if (!tab)
// 		return ;
// 	i = 0;
// 	while (tab[i])
// 	{
// 		free(tab[i]);
// 		i++;
// 	}
// 	free(tab);
// }

// // Duplication environnement
// char	**ft_env_dup(char **envp)
// {
// 	int		i;
// 	char	**envcp;

// 	i = 0;
// 	while (envp[i])
// 		i++;
// 	envcp = malloc(sizeof(char *) * (i + 1));
// 	if (!envcp)
// 		return (NULL);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		envcp[i] = ft_strdup(envp[i]);
// 		if (!envcp[i])
// 		{
// 			while (--i >= 0)
// 				free(envcp[i]);
// 			free(envcp);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	envcp[i] = NULL;
// 	return (envcp);
// }

// // Free environnement
// void	ft_freeenvp(char **envcp)
// {
// 	int	i;

// 	i = 0;
// 	if (!envcp)
// 		return ;
// 	while (envcp[i])
// 	{
// 		free(envcp[i]);
// 		i++;
// 	}
// 	free(envcp);
// }

// char	*ft_srjoin3(char *s1, char *s2, char *s3)
// {
// 	char	*tmp;
// 	char	*result;

// 	tmp = ft_strjoin(s1, s2);
// 	if (!tmp)
// 		return (NULL);
// 	result = ft_strjoin(tmp, s3);
// 	free(tmp);
// 	return (result);
// }

char	*prepare_export_string(char *arg, char **envp, char **key, char **value)
{
	char	*equal;
	char	*replaced;
	char	*expanded;
	char	*tmp;

	*key = NULL;
	*value = NULL;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
		expanded = replace_all_variables(*value, envp, 0, 1);
		free(*value);
		if (expanded)
			*value = expanded;
		else
			*value = ft_strdup("");
		tmp = ft_strjoin(*key, "=");
		replaced = ft_strjoin(tmp, *value);
		free(tmp);
	}
	else
		replaced = ft_strdup(arg);
	return (replaced);
}
