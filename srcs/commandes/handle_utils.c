/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:33:16 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 13:58:20 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_arg(char *arg, char **envp)
{
	char	*varname;
	char	*value;

	if (!arg)
		return (NULL);
	if (arg[0] != '$')
		return (ft_strdup(arg));
	varname = ft_substr(arg, 1, ft_strlen(arg) - 1);
	if (!varname)
		return (NULL);
	value = get_env_value(varname, envp);
	free(varname);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

int	handle_initial_errors(char **args)
{
	int		i;
	int		status;
	char	*clean_arg;

	i = 1;
	status = 0;
	while (args[i])
	{
		clean_arg = remove_quotes_or_slash(args[i]);
		args[i] = ft_strdup(clean_arg);
		free(clean_arg);
		i++;
	}
	return (status);
}

void	process_valid(char **args, char ***envcp, int *status)
{
	int		i;
	char	*expanded;
	int		is_empty_literal;

	i = 1;
	while (args[i])
	{
		is_empty_literal = (args[i][0] == '"'
				&& args[i][1] == '"' && args[i][2] == '\0');
		expanded = replace_var(args[i], *envcp, 0, 1);
		if (is_empty_literal || (expanded && expanded[0] == '\0'
				&& args[i][0] != '$'))
		{
			ft_putstr_fd("bash: export: `': not a valid \
identifier\n", STDERR_FILENO);
			*status = 1;
		}
		else if (args[i][0] == '$' && (!expanded || expanded[0] == '\0'))
			export_no_args(*envcp);
		else
			process_export(expanded, envcp, status);
		free(expanded);
		i++;
	}
}
