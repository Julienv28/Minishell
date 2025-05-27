/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/27 16:48:15 by opique           ###   ########.fr       */
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
;
    if (!args[1])
    {
        // ft_putstr_fd("unset: not enough arguments\n", STDOUT_FILENO);
        return (0);
    }

    i = 1;
    while (args[i])
    {
        if (args[i][0] == '-' && args[i][1] != '\0') // Si l'argument commence par '-'
        {
            ft_putstr_fd("bash: ", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd(": invalid option\n", STDERR_FILENO);
            g_exit_status = 2;
            return (g_exit_status);
        }
        // Cas special avec !
        if (strchr(args[i], '!')) // Si un '!' est trouvé dans le nom de la variable
        {
            ft_putstr_fd("bash: unset: `", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd("': event not found\n", STDERR_FILENO);
            g_exit_status = 1;
            return (g_exit_status);
        }
        i++;
    }
    i = 1;
    while (args[i])
    {
        if (!is_valid_name(args[i]))
        {
            ft_putstr_fd("bash: unset: `", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            exit_status = 1;
            i++;
            continue;
        }
        // Recréation de l'env sans la variable args[i]
        size = 0;
        while ((*envcp)[size])
            size++;
        new_env = malloc(sizeof(char *) * size);
        if (!new_env)
            return (0);
        j = 0;
        k = 0;
        while ((*envcp)[j])
        {
            if (ft_strncmp((*envcp)[j], args[i], ft_strlen(args[i])) == 0 && (*envcp)[j][ft_strlen(args[i])] == '=')
                free((*envcp)[j]);
            else
                new_env[k++] = (*envcp)[j];
            j++;
        }
        new_env[k] = NULL;
        free(*envcp);
        *envcp = new_env;
        i++;
    }
    return (exit_status);
}
