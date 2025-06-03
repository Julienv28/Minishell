/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/03 17:05:18 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// La fonction est longue, il faudrna la raccourcir...
int ft_unset(char **args, char ***envcp)
{
    int i;
    int j;
    int k;
    int size;
    char **new_env;
    int exit_status = 0;


    if (!args[1])
        return (0);
    i = 1;
    while (args[i])
    {
        if (args[i][0] == '-' && args[i][1] != '\0')
        {
            printf("minishell: %s: invalid option\n", args[i]);
            g_exit_status = 2;
            return (g_exit_status);
        }
        if (strchr(args[i], '!'))
        {
            printf("minishell: unset: `%s': event not found\n", args[i]);
            g_exit_status = 1;
            return (g_exit_status);
        }
		if (!is_valid_name(args[i]))
			{
            	printf("minishell: unset: `%s': not a valid identifier\n", \
					args[i]);
				exit_status = 1;
			}
        i++;
    }
    size = 0;
    while ((*envcp)[size])
        size++;
    new_env = malloc(sizeof(char *) * (size + 1));
    if (!new_env)
        return (1);
    j = 0;
    k = 0;
    while (j < size)
    {
        int to_remove = 0;
        i = 1;
        while (args[i])
        {
			if (!is_valid_name(args[i]))
			{
				i++;
				continue ;
			}
			if (ft_strncmp((*envcp)[j], args[i], ft_strlen(args[i])) == 0
				&& (*envcp)[j][ft_strlen(args[i])] == '=')
			{
				to_remove = 1;
				break ;
			}
			i++;
        }
        if (to_remove)
            free((*envcp)[j]);
        else
            new_env[k++] = (*envcp)[j];
        j++;
    }
    new_env[k] = NULL;
    free(*envcp);
    *envcp = new_env;
    return (exit_status);
}
