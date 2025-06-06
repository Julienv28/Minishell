#include "../includes/minishell.h"

char	*expand_exit_status(char *res)
{
	char	*status;
	char	*tmp;

	status = ft_itoa(g_exit_status);
	if (!status)
		return (NULL);
	tmp = ft_strjoin(res, status);
	if (!tmp)
		return (NULL);
	free(res);
	free(status);
	return (tmp);
}

char	*handle_special_cases(char *str, char *res, t_expand *var)
{
	if (str[*var->i] == '=')
	{
		res = append_char(res, '$');
		res = append_char(res, '=');
		(*var->i)++;
		while (ft_isalnum(str[*var->i]) || str[*var->i] == '_')
			res = append_char(res, str[(*var->i)++]);
		return (res);
	}
	if (str[*var->i] == '?')
		return ((*var->i)++, expand_exit_status(res));
	if (ft_isdigit(str[*var->i]))
		return ((*var->i)++, res);
	if (str[*var->i] == '"'
		&& (ft_isalpha(str[*var->i + 1]) || str[*var->i + 1] == '_'))
	{
		res = append_char(res, '$');
		res = append_char(res, str[(*var->i)++]);
	}
	return (res);
}

char	*handle_brace_variable(char *str, char *res, t_expand *var)
{
	char	var_name[256];
	char	*env;
	char	*tmp;
	int		j;

	(*var->i)++;
	j = 0;
	while (str[*var->i] && str[*var->i] != '}' && j < 255)
		var_name[j++] = str[(*var->i)++];
	var_name[j] = '\0';
	if (str[*var->i] == '}')
		(*var->i)++;
	env = get_env_value(var_name, var->envcp);
	if (!env)
		env = "";
	tmp = ft_strjoin(res, env);
	free(res);
	return (tmp);
}

char	*handle_quote(char *str, char *res, t_expand *var)
{
	if (str[*var->i] == '"' && str[*var->i + 1] == '"')
	{
		(*var->i) += 2;
		return (append_char(res, '\0'));
	}
	if (str[*var->i] == '"' && !str[*var->i + 1])
		return ((*var->i)++, append_char(res, '$'));
	if (str[*var->i] == '"')
	{
		(*var->i)++;
		while (str[*var->i] && str[*var->i] != '"')
			res = append_char(res, str[(*var->i)++]);
		if (str[*var->i] == '"')
			(*var->i)++;
		return (res);
	}
	(*var->i)++;
	while (str[*var->i] && str[*var->i] != '\'')
		res = append_char(res, str[(*var->i)++]);
	if (str[*var->i] == '\'')
		(*var->i)++;
	return (res);
}

char	*get_variable_name(char *str, t_expand *var, char *var_name)
{
	int	j;

	j = 0;
	while (str[*var->i] && (ft_isalnum(str[*var->i]) || str[*var->i] == '_'))
		var_name[j++] = str[(*var->i)++];
	var_name[j] = '\0';
	return (var_name);
}
