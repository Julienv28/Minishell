/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollard.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:25:11 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 15:00:13 by juvitry          ###   ########.fr       */
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
	variable_name(str, var, var_name);
	env_value = get_expanded_value(var_name, var, &need_free);
	if (!env_value)
		return (NULL);
	tmp = ft_strjoin(res, env_value);
	if (!tmp)
	{
		ft_putstr_fd("[ERROR] ft_strjoin failed\n", STDERR_FILENO);
		return (NULL);
	}
	free(res);
	if (need_free)
		free(env_value);
	return (tmp);
}

static char	*expand_loop(char *str, char *res, t_msh *msh)
{
	int	quotes[2];

	quotes[0] = 0;
	quotes[1] = 0;
	while (str[*msh->var->i])
	{
		if (str[*msh->var->i] == '\\' && str[*msh->var->i + 1] == '$')
		{
			res = append_char(res, '$');
			if (!res)
				return (NULL);
			(*msh->var->i) += 2;
			continue ;
		}
		res = quote_dol(str, res, msh, quotes);
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*replace_var(char *s, t_msh *msh, int is_hd, int expand)
{
	int			i;
	char		*res;

	if (!msh->var)
	{
    	msh->var = malloc(sizeof(t_expand));
    	if (!msh->var)
        	return (NULL);
	}
	i = 0;
	msh->var->envcp = msh->envcp;
	msh->var->i = &i;
	msh->var->is_heredoc = is_hd;
	msh->var->quoted = 0;
	msh->var->expand_vars = expand;
	if (!s)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	return (expand_loop(s, res, msh));
}
