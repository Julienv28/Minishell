/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/22 16:49:54 by juvitry          ###   ########.fr       */
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

//La fonction est longue, il faudra la raccourcir...
void	ft_unset(char *key, char ***envcp)
{
	int		i;
	int		j;
	int		size;
	char	**new_env;

	if (!key || !*envcp)
	{
		ft_putstr_fd("unset: not enough arguments\n", STDOUT_FILENO);
		return ;
	}
	size = 0;
	while ((*envcp)[size])
		size++;
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	if (key[0] == '\0')
	{
		ft_putstr_fd("bash: unset: `': not a valid identifier\n", STDERR_FILENO);
		return ;
	}
	if (key[0] == '-')
	{
		printf("bash: unset: -%c: invalid option\n", key[1]);
		return ;
	}
	if (!is_valid_name(key))
	{
		ft_putstr_fd("bash: unset: `", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return ;
	}
	while ((*envcp)[i])
	{
		if (ft_strncmp((*envcp)[i], key, ft_strlen(key)) == 0
			&& (*envcp)[i][ft_strlen(key)] == '=')
		{
			free((*envcp)[i]);
			i++;
			continue ;
		}
		new_env[j++] = (*envcp)[i++];
	}
	new_env[j] = NULL;
	free(*envcp);
	*envcp = new_env;
}

/*Le continue ; 
ici dans la boucle while ((*envcp)[i]) signifie :

    « Passe à l’itération suivante de la boucle, sans faire le reste du code 
	(ici, sans copier la ligne dans new_env) »*/