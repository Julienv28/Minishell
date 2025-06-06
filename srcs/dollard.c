#include "../includes/minishell.h"

char	*expand_env_variable(char *str, char *res, t_expand *var)
{
	char	var_name[256];
	char	*tmp;
	char	*env_value;
	int		need_free;

    printf("ENTER expand_env_variable\n");
    if (!str || !res || !var)
	{
		printf("[ERROR] NULL pointer: str=%p, res=%p, var=%p\n", (void*)str, (void*)res, (void*)var);
		return (NULL);
	}
	need_free = 0;
    if (!var || !var->envcp)
    {
        printf("[ERROR] var or var->envcp is NULL\n");
        return NULL;
    }
    printf("Calling get_variable_name\n");
	get_variable_name(str, var, var_name);
    printf("[DEBUG] expand_env_variable: detected var '$%s'\n", var_name);
	if (var->quoted)
		env_value = get_env_value(var_name, var->envcp);
	else
		env_value = get_value_cleaned(var_name, var->envcp);
        printf("env_value = %p (%s)\n", (void*)env_value, env_value ? env_value : "NULL");

    // DEBUG: Valeur de la variable
	if (env_value)
        printf("[DEBUG] expand_env_variable: value = '%s'\n", env_value);
    else
        printf("[DEBUG] expand_env_variable: variable '%s' not found\n", var_name);


    if (!env_value)
        env_value = "";
    else if (!var->quoted)
        need_free = 1;
	tmp = ft_strjoin(res, env_value);
	if (!tmp)
	{
		printf("[ERROR] ft_strjoin failed\n");
		return NULL;
	}
	free(res);
	if (need_free)
		free(env_value);
	return (tmp);
}

char	*replace_variable_or_special(char *str, char *res, t_expand *var)
{
	(*var->i)++;
	if (!str[*var->i])
		return (append_char(res, '$'));
	if (var->is_heredoc)
	{
		if (str[*var->i] != '"')
			return (expand_env_variable(str, res, var));
		else
			return (append_char(res, '$'));
	}
	if (str[*var->i] == '"' || str[*var->i] == '\'')
        return (handle_quote(str, res, var));
	if (str[*var->i] == '{')
		return (handle_brace_variable(str, res, var));
	if (ft_isalpha(str[*var->i]) || str [*var->i] == '_')
		return (expand_env_variable(str, res, var));
	return (handle_special_cases(str, res, var));
}

char	*handle_quotes_and_dollar(char *str, char *res,
		t_expand *var, int *quotes)
{
	if (str[*var->i] == '\'' && !quotes[1])
	{
		quotes[0] = !quotes[0];
        printf("[DEBUG] Toggled single quote mode to %d at index %d\n", quotes[0], *var->i);
		(*var->i)++;
		return (res);
	}
	if (str[*var->i] == '"' && !quotes[0])
	{
		quotes[1] = !quotes[1];
        printf("[DEBUG] Toggled double quote mode to %d at index %d\n", quotes[1], *var->i);
		(*var->i)++;
		return (res);
	}
    //if (str[*var->i] == '$' && !quotes[0] && !var->is_heredoc)s

    printf("[DEBUG] Dollar test: char='%c', quoted=%d, is_heredoc=%d, quotes[0]=%d, quotes[1]=%d, index=%d\n",
        str[*var->i], var->quoted, var->is_heredoc, quotes[0], quotes[1], *var->i);

    if (str[*var->i] == '$' && !quotes[0] && var->expand_vars)
	{
		var->quoted = quotes[1];
        printf("[DEBUG] Expanding variable at index %d (heredoc: %d, quoted: %d)\n",
			*var->i, var->is_heredoc, var->quoted);
		return (replace_variable_or_special(str, res, var));
	}
	res = append_char(res, str[*var->i]);
	if (!res)
		return (NULL);
	(*var->i)++;
	return (res);
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
            printf("[DEBUG] Found escaped dollar sign at index %d\n", *var->i);
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
    printf("[DEBUG] Final expanded result = '%s'\n", res);
	return (res);
}

char	*replace_all_variables(char *str, char **envcp, int is_heredoc, int expand_vars)
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
    printf("[DEBUG] replace_all_variables: input = '%s', is_heredoc = %d, expand_vars = %d\n",
            str, is_heredoc, expand_vars);
    res = ft_strdup("");
	if (!res)
		return (NULL);
	return (expand_loop(str, res, &var));
}

/*
char	*expand_env_variable(char *str, char *res, t_expand *var)
{
	char	var_name[256];
	int		j;
	char	*tmp;
	char	*env_value;
	int		need_free;

    j = 0;
    need_free = 0;
    while (str[*var->i] && (ft_isalnum(str[*var->i]) || str[*var->i] == '_'))
        var_name[j++] = str[(*var->i)++];
    var_name[j] = '\0';
	if (var->quoted)
		env_value = get_env_value(var_name, var->envcp);
	else
	{
		env_value = get_value_cleaned(var_name, var->envcp);
		need_free = 1;
	}
	if (!env_value)
    {
		env_value = "";
        need_free = 0;
    }
	tmp = ft_strjoin(res, env_value);
	if (!tmp)
		return (NULL);
	free(res);
	if (need_free)
		free(env_value); // SEGFAULT ICI
	return (tmp);
}*/

/*
char *replace_variable_or_special(char *str, char *res, t_expand *var)
{
    char    *env;
    char    *tmp;
    char    var_name[256];
    int     j;

    (*var->i)++;
    if (!str[*var->i])
        return (append_char(res, '$'));
     if (var->is_heredoc && str[*var->i] != '"')
        return (expand_env_variable(str, res, var));
    if (var->is_heredoc)
    {
        printf("Heredoc mode with quotes, no variable expansion\n");
        return (append_char(res, '$'));
    }
    if (str[*var->i] == '"' && str[*var->i + 1] == '"')
     {
        (*var->i) += 2;                    // Skip les guillemets vides
        return (append_char(res, '\0'));
    }
    if (str[*var->i] == '"' && !str[*var->i + 1])
    {
        (*var->i)++; // Skip
        return (append_char(res, '$'));
    }
    if (str[*var->i] == '"')
    {
        (*var->i)++; // Skip opening "
        while (str[*var->i] && str[*var->i] != '"')
            res = append_char(res, str[(*var->i)++]);
        if (str[*var->i] == '"')
            (*var->i)++; // Skip closing "
        return (res);
    } 
    // Cas pour $'...' (ANSI-C quotes)
    if (str[*var->i] == '\'')
    {
        (*var->i)++; // Skip opening '
        while (str[*var->i] && str[*var->i] != '\'')
            res = append_char(res, str[(*var->i)++]);
        if (str[*var->i] == '\'')
            (*var->i)++; // Skip closing '
        return (res);
    }
    if (str[*var->i] == '{')
    {
        (*var->i)++; // Skip {
        j = 0;
        while (str[*var->i] && str[*var->i] != '}' && j < 255)
            var_name[j++] = str[(*var->i)++];
        var_name[j] = '\0';
        if (str[*var->i] == '}')
            (*var->i)++; // Skip closing }
        env = get_env_value(var_name, var->envcp);
        if (!env)
            env = "";
        tmp = ft_strjoin(res, env);
        free(res);
        return (tmp);
    }
    if (ft_isalpha(str[*var->i]) || str[*var->i] == '_')
        return (expand_env_variable(str, res, var));
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
    {
        (*var->i)++;
        return (expand_exit_status(res));
    }
    if (ft_isdigit(str[*var->i]))
        return ((*var->i)++, res);
    if (str[*var->i] == '"' && (ft_isalpha(str[*var->i + 1]) ||
     str[*var->i + 1] == '_'))
    {
        res = append_char(res, '$');
        res = append_char(res, str[(*var->i)++]);
    var.envcp = envcp;
        return (res);
    }
    return (res);
}*/

/*
char *replace_all_variables(char *str, char **envcp, int is_heredoc)
{
    t_expand var;
    int i = 0;
    char *res;
    int in_single_quote = 0;
    int in_double_quote = 0;

    var.envcp = envcp;
    var.i = &i;
    var.is_heredoc = is_heredoc;
    var.quoted = 0;

    res = ft_strdup("");
    if (!str || !res)
        return NULL;
    while (str[*var.i])
    {
        if (str[*var.i] == '\\' && str[*var.i + 1] == '$')
        {
            res = append_char(res, '$');
            if (!res)
                return NULL;
            (*var.i) += 2;
            continue;
        }
        if (str[*var.i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            (*var.i)++;
            continue ;
        }
        else if (str[*var.i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            (*var.i)++;
            continue;
        }
        else if (str[*var.i] == '$' && !in_single_quote && !var.is_heredoc)
        {
            var.quoted = in_double_quote;
            res = replace_variable_or_special(str, res, &var);
        }
        else
        {
            res = append_char(res, str[*var.i]);
            if (!res)
                return NULL;
            (*var.i)++;
        }
    }
    return res;
}*/

// N EST PAS UTILISER
/*
// Fonction pour remplacer toutes les variables d'environnement dans un 
tableau d'arguments
void expand_variables(char **args, char **envcp, int is_heredoc)
{
    int     i;
    int     j;
    char    *tmp;
    char    *res;
    char    *new_args[1024];
    t_expand    var;
    
    var.envcp = envcp;
    var.is_heredoc = is_heredoc;
    var.i = &i;
    i = 0;
    j = 0;
    while (args[i])
    {
        tmp = replace_all_variables(args[i], &var);
        if (!tmp)
        {
            i++;
            continue ;
        }
        res = remove_quotes_or_slash(tmp);
        free(tmp);
        if (!res)
        {
            i++;
            continue ;
        }
        if (res[0] == '\0' &&
            !strchr(args[i], '"') &&
            !strchr(args[i], '\''))
        {
            free(res);
            i++;
            continue ;
        }
        new_args[j++] = res;
        i++;
	}
	new_args[j] = NULL;
	i = 0;
	while (args[i])
		free(args[i++]);
	args[i] = NULL;
	i = 0;
	while (new_args[i])
	{
		args[i] = ft_strdup(new_args[i]);
		if (!args[i])
			break ; // nouvelle copie pour éviter ownership multiple
        free(new_args[i]);
        i++;
	}
	args[i] = NULL;
} */