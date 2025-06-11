/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollard.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:25:11 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 14:41:17 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_expanded_value(char *var_name, t_expand *var, int *need_free)
{
	char	*env_value;

	if (var->quoted)
	{
		env_value = get_env_value(var_name, var->envcp);
		if (env_value == NULL)
			return (NULL);
		*need_free = 0;
	}
	else
	{
		env_value = get_value_cleaned(var_name, var->envcp);
		if (!env_value)
			env_value = "";
		else
			*need_free = 1;
	}
	return (env_value);
}

char	*expand_env_variable(char *str, char *res, t_expand *var)
{
	char	var_name[256];
	char	*tmp;
	char	*env_value;
	int		need_free;

	if (!var || !var->envcp)
		return (NULL);
	need_free = 0;
	get_variable_name(str, var, var_name);
	env_value = get_expanded_value(var_name, var, &need_free);
	if (!env_value)
		return (NULL);
	tmp = ft_strjoin(res, env_value);
	if (!tmp)
		return (printf("[ERROR] ft_strjoin failed\n"), NULL);
	free(res);
	if (need_free)
		free(env_value);
	return (tmp);
}

char	*expand_loop(char *str, char *res, t_expand *var)
{
	int	quotes[2];

	quotes[0] = 0;
	quotes[1] = 0;
	while (str[*var->i])
	{
		if (str[*var->i] == '\\' && str[*var->i + 1] == '$')
		{
			res = append_char(res, '$');
			if (!res)
				return (NULL);
			(*var->i) += 2;
			continue ;
		}
		res = handle_quotes_and_dollar(str, res, var, quotes);
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*replace_all_variables(char *str, char **envcp,
		int is_heredoc, int expand_vars)
{
	t_expand	var;
	int			i;
	char		*res;

	i = 0;
	var.envcp = envcp;
	var.i = &i;
	var.is_heredoc = is_heredoc;
	var.quoted = 0;
	var.expand_vars = expand_vars;
	if (!str)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	return (expand_loop(str, res, &var));
}
