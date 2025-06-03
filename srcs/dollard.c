#include "../includes/minishell.h"

// Ajoute un caractère simple à la chaîne résultante
char	*append_char(char *res, char c)
{
	char	current[2];
	char	*tmp;

	current[0] = c;
	current[1] = '\0';
	tmp = ft_strjoin(res, current);
	if (!tmp)
		return (NULL);
	free(res);
	return (tmp);
}

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

char	*expand_env_variable(char *str, int *i, char *res, char **envcp, int quoted)
{
	char	var_name[256];
	int		j = 0;
	char	*tmp;
	char	*env_value;
	int		need_free = 0;

    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        var_name[j++] = str[(*i)++];
    var_name[j] = '\0';
    printf("Expanding env variable: %s\n", var_name); 
    // Choix entre get_env_value ou get_value_cleaned selon le flag quoted
	if (quoted)
		env_value = get_env_value(var_name, envcp);
	else
	{
		env_value = get_value_cleaned(var_name, envcp);
		need_free = 1;
	}
	if (!env_value)
		env_value = ""; // En cas d'absence de variable
	tmp = ft_strjoin(res, env_value);
	if (!tmp)
		return (NULL);
	free(res);
	if (need_free)
		free(env_value);
	return (tmp);
}

char *replace_variable_or_special(char *str, int *i, char *res, char **envcp, int is_heredoc)
{
    char    *env;
    char    *tmp;
    char    var_name[256];
    int     j;

    (*i)++;

    if (!str[*i])
        return (append_char(res, '$'));
    if (is_heredoc && str[*i] != '"')
        return (expand_env_variable(str, i, res, envcp, is_heredoc));
    if (is_heredoc)
    {
        printf("Heredoc mode with quotes, no variable expansion\n");
        return (append_char(res, '$'));
    }
    
    // Cas pour $"" (chaîne vide entre guillemets) → chaîne vide, donc ne rien ajouter
    if (str[*i] == '"' && str[*i + 1] == '"')
    {
        (*i) += 2;                    // Skip les guillemets vides
        return (append_char(res, '\0'));
    }

    // Cas pour $"" suivi d'un autre " (chaîne vide) → chaîne vide mais $ reste
    if (str[*i] == '"' && !str[*i + 1])
    {
        (*i)++; // Skip
        return (append_char(res, '$'));
    }

    // Cas pour $"..." (gettext avec contenu)
    if (str[*i] == '"')
    {
        (*i)++; // Skip opening "
        while (str[*i] && str[*i] != '"')
            res = append_char(res, str[(*i)++]);
        if (str[*i] == '"')
            (*i)++; // Skip closing "
        return (res);
    }

    // Cas pour $'...' (ANSI-C quotes)
    if (str[*i] == '\'')
    {
        (*i)++; // Skip opening '
        while (str[*i] && str[*i] != '\'')
            res = append_char(res, str[(*i)++]);
        if (str[*i] == '\'')
            (*i)++; // Skip closing '
        return (res);
    }

    // Cas pour ${VAR}
    if (str[*i] == '{')
    {
        (*i)++; // Skip {
        j = 0;
        while (str[*i] && str[*i] != '}' && j < 255)
            var_name[j++] = str[(*i)++];
        var_name[j] = '\0';
        if (str[*i] == '}')
            (*i)++; // Skip closing }

        env = get_env_value(var_name, envcp);
        if (!env)
            env = "";
        tmp = ft_strjoin(res, env);
        free(res);
        return (tmp);
    }

    // Cas pour $HOME, $USER, etc.
    // Cette section gère les variables d'environnement simples, sans délimiteurs spéciaux.
    if (ft_isalpha(str[*i]) || str[*i] == '_')
        return (expand_env_variable(str, i, res, envcp, is_heredoc));

    // Cas où on a un égal : $=HOME, ce n'est pas une expansion de variable, on garde le $
    if (str[*i] == '=')
    {
        res = append_char(res, '$');
        res = append_char(res, '=');
        (*i)++;
        while (ft_isalnum(str[*i]) || str[*i] == '_')
            res = append_char(res, str[(*i)++]);
        return (res);
    }

    // Cas pour $? (status de sortie)
    if (str[*i] == '?')
    {
        (*i)++;
        return (expand_exit_status(res));
    }

    // Cas pour $ suivi d'un chiffre → pas d'expansion (ex: $1)
    if (ft_isdigit(str[*i]))
    {
        (*i)++;
        return (res);
    }
    if (str[*i] == '"' && (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_'))
    {
        res = append_char(res, '$');
        res = append_char(res, str[(*i)++]);
        return (res);
    }
    return (res);
}

char	*replace_all_variables(char *str, char **envcp, int is_heredoc)
{
	int		i = 0;
	char	*res = ft_strdup("");
	int		in_single_quote = 0;
	int		in_double_quote = 0;
	int		quoted = 0;

	if (!res || !str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '$')
		{
			res = append_char(res, '$');
			if (!res)
				return (NULL);
			i += 2;
			continue ;
		}
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
			continue ;
        }
        else if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            i++;
			continue ;
        }
        else if (str[i] == '$' && !in_single_quote && !is_heredoc)
        {
            quoted = in_double_quote;
            res = replace_variable_or_special(str, &i, res, envcp, quoted);
        }
        else
        {
            res = append_char(res, str[i++]);
			if (!res)
				return (NULL);
        }
    }
    return (res);
}

// Fonction pour remplacer toutes les variables d'environnement dans un tableau d'arguments
void expand_variables(char **args, char **envcp, int is_heredoc)
{
    int     i = 0;
    int     j = 0;
    char    *tmp;
    char    *res;
    char    *new_args[1024];
    
    while (args[i])
    {
        tmp = replace_all_variables(args[i], envcp, is_heredoc);
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
}
