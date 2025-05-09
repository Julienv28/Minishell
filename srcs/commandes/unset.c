/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/09 18:12:40 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*UNSET = fonction qui supprime une variable de la liste des env.
exemple :
COLORTERM=truecolor
XAUTHORITY=/home/juvitry/.Xauthority
PWD=/home/juvitry/Documents/minishell/minigit3
AZ=azerty
-> unset AZ
COLORTERM=truecolor
XAUTHORITY=/home/juvitry/.Xauthority
PWD=/home/juvitry/Documents/minishell/minigit3
et AZ a disparu !
unset seul retourne:
unset: not enough arguments
*/

// La fonction est longue, il faudra la raccourcir...
void ft_unset(char **args, char ***envcp)
{
    int i;
    int j;
    int k;
    int size;
    char **new_env;

    if (!args[1])
    {
        // ft_putstr_fd("unset: not enough arguments\n", STDOUT_FILENO);
        return;
    }

    i = 1;
    while (args[i])
    {
        if (args[i][0] == '-' && args[i][1] != '\0') // Si l'argument commence par '-'
        {
            ft_putstr_fd("bash: ", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd(": invalid option\n", STDERR_FILENO);
            return;
        }
        // Cas special avec !
        if (strchr(args[i], '!')) // Si un '!' est trouvé dans le nom de la variable
        {
            ft_putstr_fd("bash: unset: `", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd("': event not found\n", STDERR_FILENO);
            return;
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
            i++;
            continue;
        }
        // Recréation de l'env sans la variable args[i]
        size = 0;
        while ((*envcp)[size])
            size++;
        new_env = malloc(sizeof(char *) * size);
        if (!new_env)
            return;
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
}
/*Le continue ;
ici dans la boucle while ((*envcp)[i]) signifie :

    « Passe à l’itération suivante de la boucle, sans faire le reste du code
    (ici, sans copier la ligne dans new_env) »*/