/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:29:59 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 16:20:47 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_builtin(char **args, char ***envcp, t_com_list *cmd)
{
	ft_freeenvp(*envcp);
	ft_exit(args, 1, cmd);
	return (0);
}

// int handle_export(char **args, char ***envcp)
// {
//     int has_error = 0;
//     int has_valid_export = 0;
//     int status = 0;

//     // Aucun argument : affiche la liste
//     if (!args[1])
//         return export_no_args(*envcp);

//     for (int i = 1; args[i]; i++)
//     {
//         // Argument littéralement vide
//         if (args[i][0] == '\0')
//         {
//             fprintf(stderr, "bash: export: `%s': not a valid identifier\n", args[i]);
//             has_error = 1;
//             continue;
//         }

//         // Expansion des variables
//         char *expanded = replace_all_variables(args[i], *envcp, 0, 1);
//         if (!expanded)
//         {
//             // Erreur d'expansion
//             fprintf(stderr, "bash: export: `%s': not a valid identifier\n", args[i]);
//             has_error = 1;
//             continue;
//         }

//         if (expanded[0] == '\0')
//         {
//             // Variable non définie, on ne fait rien
//             free(expanded);
//             continue;
//         }

//         // Export valide
//         process_export_entry(expanded, envcp, &status);
//         has_valid_export = 1;
//         free(expanded);
//     }

//     // Si aucun export valide et pas d'erreur : affiche la liste
//     if (!has_valid_export && !has_error)
//         status = export_no_args(*envcp);
//     // En cas d'erreur, code retour = 1
//     else if (has_error)
//         status = 1;

//     return status;
// }


int	handle_export(char **args, char ***envcp)
{
	int	exit_status;

	if (!args[1])
		return (export_no_args(*envcp));
	if (check_events(args[1]) == 1)
		return (1);
	exit_status = handle_initial_errors(args);
	process_valid_exports(args, envcp, &exit_status);
	return (exit_status);
}

static int	handle_env(char **args, char ***envcp)
{
	if (args[1] && args[1][0] == '-')
	{
		printf("minishell: env: -%c: invalid option\n", args[1][1]);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (ft_env(*envcp), 0);
}

static int	handle_unset(char **args, char ***envcp)
{
	if (!args[1])
		return (0);
	if (check_events(args[1]) == 0)
		return (ft_unset(args, envcp));
	return (1);
}

int	exec_builting(char **args, char ***envcp, t_com_list *cmd)
{
	if (ft_strcmp(args[0], "exit") == 0)
		return (handle_exit_builtin(args, envcp, cmd));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, envcp));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args, envcp));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, envcp), 0);
	else if (ft_strcmp(args[0], "export") == 0)
		return (handle_export(args, envcp));
	else if (ft_strcmp(args[0], "env") == 0)
		return (handle_env(args, envcp));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (handle_unset(args, envcp));
	return (g_exit_status);
}
