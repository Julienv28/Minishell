/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:01 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 11:25:25 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*build_spaced_result(char *arg, int i, int len, char **envcp)
{
	char	*before;
	char	*var_key;
	char	*value;
	char	*spaced;
	char	*res;

	before = ft_substr(arg, 0, i);
	if (!before)
		return (NULL);
	var_key = ft_substr(arg + i + 1, 0, len);
	if (!var_key)
		return (free(before), NULL);
	value = get_value_cleaned(var_key, envcp);
	if (!value)
		return (free_all(before, var_key, NULL, NULL), NULL);
	spaced = ft_strjoin(before, " ");
	if (!spaced)
		return (free_all(before, var_key, NULL, value), NULL);
	res = ft_strjoin(spaced, value);
	if (!res)
		return (free_all(before, var_key, spaced, value), NULL);
	return (free_all(before, var_key, spaced, value), res);
}

int	is_valid_var(const char *arg, int i)
{
	if (arg[i] == '$' && i > 0 && !ft_isspace(arg[i - 1]))
		return (1);
	return (0);
}

char	*add_space_if_needed(char *arg, char **envcp)
{
	int		i;
	int		len;
	char	*var_name;

	i = 0;
	while (arg[i])
	{
		if (is_valid_var(arg, i))
		{
			var_name = arg + i + 1;
			len = 0;
			while (var_name[len] && (ft_isalnum(var_name[len])
					|| var_name[len] == '_'))
				len++;
			if (len > 0)
				return (build_spaced_result(arg, i, len, envcp));
		}
		i++;
	}
	return (ft_strdup(arg));
}

static void	print_echo_arg(char *arg, char ***envcp)
{
	char	*unescaped;
	char	*patched;

	unescaped = unescape_backslashes(arg);
	if (!unescaped)
		return ;
	patched = add_space_if_needed(unescaped, *envcp);
	free(unescaped);
	if (patched)
	{
		ft_putstr_fd(patched, STDOUT_FILENO);
		free(patched);
	}
}

void	ft_echo(char **args, char ***envcp)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		print_echo_arg(args[i], envcp);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

/*
char	*add_space_if_needed(char *arg, char **envcp)
{
	int		i;
	char	*var_name;
	int		len;
	char	*before;
	char	*var_key;
	char	*value;
	char	*spaced;
	char	*res;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && i > 0 && !ft_isspace(arg[i -1]))
		{
			var_name = arg + i + 1;
			len = 0;
			while (var_name[len] && (ft_isalnum(var_name[len])
					|| var_name[len] == '_'))
				len++;
			if (len > 0)
			{
				before = ft_substr(arg, 0, i);
				if (!before)
					return (NULL);
				var_key = ft_substr(var_name, 0, len);
				if (!var_key)
				{
					free (before);
					return (NULL);
				}
				value = get_value_cleaned(var_key, envcp);
				if (!value)
				{
					value = ft_strdup("");
					if (!value)
					{
						free(before);
						free(var_key);
						return (NULL);
					}
				}
				spaced = ft_strjoin(before, " ");
				if (!spaced)
				{
					free(before);
					free(var_key);
					free(value);
					return (NULL);
				}
				res = ft_strjoin(spaced, value);
				if (!res)
				{
					free(before);
					free(var_key);
					free(spaced);
					free(value);
					return (NULL);
				}
				free (before);
				free (spaced);
				free (var_key);
				free (value);
				return (res);
			}
		}
		i++;
	}
	return (ft_strdup(arg));
}*/
