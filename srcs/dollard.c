#include "../includes/minishell.h"

// Ajoute un caractère simple à la chaîne résultante
char *append_char(char *res, char c)
{
    char current[2];
    char *tmp;

    current[0] = c;
    current[1] = '\0';
    // printf("Appending char: '%c'\n", c);
    tmp = ft_strjoin(res, current);
    free(res);
    return tmp;
}

char *expand_exit_status(char *res)
{
    char *status = ft_itoa(g_exit_status);
    char *tmp;

    if (!status)
        return NULL;
    printf("Expanding exit status: %s\n", status);
    tmp = ft_strjoin(res, status);
    free(res);
    free(status);
    return tmp;
}

char *expand_env_variable(char *str, int *i, char *res, char **envcp, int quoted)
{
    char var_name[256];
    int j = 0;
    char *tmp;
    char *env_value;

    // Extraction du nom de la variable d'environnement
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        var_name[j++] = str[(*i)++];
    var_name[j] = '\0';

    printf("Expanding env variable: %s\n", var_name); 
    // Choix entre get_env_value ou get_value_cleaned selon le flag quoted
    if (quoted)
        env_value = get_env_value(var_name, envcp); // On garde les espaces dans le cas de la citation
    else
        env_value = get_value_cleaned(var_name, envcp); // On nettoie les espaces si non cité

    if (!env_value)
        env_value = ""; // En cas d'absence de variable

    tmp = ft_strjoin(res, env_value); // Concaténation avec le résultat
    free(res);
    return tmp;
}

char *replace_variable_or_special(char *str, int *i, char *res, char **envcp, int is_heredoc)
{
    printf("is_heredoc = %d\n", is_heredoc);
    (*i)++; // Skip $

    if (!str[*i])
        return append_char(res, '$');
    // printf("Processing char after $: '%c'\n", str[*i]);
    // Si c'est un heredoc, sans guillemet
    if (is_heredoc && str[*i] != '"')
        return expand_env_variable(str, i, res, envcp, is_heredoc);
    if (is_heredoc)
    {
        printf("Heredoc mode with quotes, no variable expansion\n");
        // return append_char(res, '$');
    }
    
    // Cas pour $"" (chaîne vide entre guillemets) → chaîne vide, donc ne rien ajouter
    if (str[*i] == '"' && str[*i + 1] == '"')
    {
        (*i) += 2;                    // Skip les guillemets vides
        return append_char(res, '\0');
    }

    // Cas pour $"" suivi d'un autre " (chaîne vide) → chaîne vide mais $ reste
    if (str[*i] == '"' && !str[*i + 1])
    {
        (*i)++; // Skip
        return append_char(res, '$');
    }

    // Cas pour $"..." (gettext avec contenu)
    if (str[*i] == '"')
    {
        (*i)++; // Skip opening "
        while (str[*i] && str[*i] != '"')
            res = append_char(res, str[(*i)++]);
        if (str[*i] == '"')
            (*i)++; // Skip closing "
        return res;
    }

    // Cas pour $'...' (ANSI-C quotes)
    if (str[*i] == '\'')
    {
        (*i)++; // Skip opening '
        while (str[*i] && str[*i] != '\'')
            res = append_char(res, str[(*i)++]);
        if (str[*i] == '\'')
            (*i)++; // Skip closing '
        return res;
    }

    // Cas pour ${VAR}
    if (str[*i] == '{')
    {
        (*i)++; // Skip {
        char var_name[256];
        int j = 0;
        while (str[*i] && str[*i] != '}' && j < 255)
            var_name[j++] = str[(*i)++];
        var_name[j] = '\0';
        if (str[*i] == '}')
            (*i)++; // Skip closing }

        char *env = get_env_value(var_name, envcp);
        if (!env)
            env = "";
        char *tmp = ft_strjoin(res, env);
        free(res);
        return tmp;
    }

    // Cas pour $HOME, $USER, etc.
    // Cette section gère les variables d'environnement simples, sans délimiteurs spéciaux.
    if (ft_isalpha(str[*i]) || str[*i] == '_')
        return expand_env_variable(str, i, res, envcp, is_heredoc);

    // Cas où on a un égal : $=HOME, ce n'est pas une expansion de variable, on garde le $
    if (str[*i] == '=')
    {
        res = append_char(res, '$');
        res = append_char(res, '=');
        (*i)++;
        while (ft_isalnum(str[*i]) || str[*i] == '_')
            res = append_char(res, str[(*i)++]);
        return res;
    }

    // Cas pour $? (status de sortie)
    if (str[*i] == '?')
    {
        (*i)++;
        return expand_exit_status(res);
    }

    // Cas pour $ suivi d'un chiffre → pas d'expansion (ex: $1)
    if (ft_isdigit(str[*i]))
    {
        (*i)++;
        return res;
    }

    // Cas où on a un $ suivi d'un guillemet et d'une lettre ou d'un caractère spécial
    if (str[*i] == '"' && (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_'))
    {
        // Le $ reste littéral dans ce cas-là (exemple: "$HOME")
        res = append_char(res, '$');
        res = append_char(res, str[(*i)++]);
        return res;
    }

    return res;
}

/*
char *replace_variable_or_special(char *str, int *i, char *res, char **envcp, int quoted)
{
    (*i)++; // Skip the $

    if (!str[*i])
        return append_char(res, '$'); // Si rien ne suit, on garde le $

    // Si la variable est suivie directement de caractères non espacés, gérer correctement l'expansion.
    if (ft_isalpha(str[*i]) || str[*i] == '_')
    {
        return expand_env_variable(str, i, res, envcp, quoted);
    }
    // Cas pour $ suivi de guillemets doubles (doit juste ajouter un $)
    if (str[*i] == '"')
    {
        res = append_char(res, '$'); // On garde le $
        return res;
    }
    // Cas pour $"" (chaîne vide entre guillemets)
    if (str[*i] == '"' && str[*i + 1] == '"')
    {
        (*i) += 2; // Ignorer les guillemets vides et ne rien ajouter
        return res;
    }

    // Cas pour $ suivi de guillemets simples (pas d'expansion)
    if (str[*i] == '\'' || str[*i] == '"')
    {
        return res; // On ne fait pas d'expansion ici
    }

    // Cas pour $ suivi de {foo} (exemple ${HOME})
    if (str[*i] == '{')
    {
        (*i)++; // Skip the opening '{'
        while (str[*i] && str[*i] != '}')
        {
            res = append_char(res, str[(*i)++]);
        }
        if (str[*i] == '}')
        {
            (*i)++; // Skip the closing '}'
        }
        return res;
    }

    // Gestion de $'' ou $"" (quote ANSI C ou gettext)
    if ((str[*i] == '\'' || str[*i] == '"') && (*i == 1 || str[*i - 1] == '$'))
    {
        char quote = str[*i];
        (*i)++; // skip opening quote

        while (str[*i] && str[*i] != quote)
            res = append_char(res, str[(*i)++]);

        if (str[*i] == quote)
            (*i)++; // skip closing quote

        return res; // PAS de $ ajouté
    }

    // Cas pour $=HOME
    if (str[*i] == '=')
    {
        res = append_char(res, '$');
        res = append_char(res, '=');
        (*i)++;
        while (ft_isalnum(str[*i]) || str[*i] == '_')
            res = append_char(res, str[(*i)++]);
        return res;
    }

    // Gérer cas "$"
    if (str[*i] == '"' || str[*i] == '\'')
    {
        res = append_char(res, '$'); // on garde le $
        return res;
    }
    if (str[*i] == '?')
    {
        (*i)++;
        return expand_exit_status(res);
    }

    if (ft_isdigit(str[*i]))
    {
        (*i)++;
        return res;
    }

    return expand_env_variable(str, i, res, envcp, quoted);
}*/

/*
char *replace_all_variables(char *str, char **envcp)
{
    int i = 0;
    char *res = ft_strdup("");
    if (!res || !str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            res = append_char(res, str[i++]);
            while (str[i] && str[i] != '\'')
                res = append_char(res, str[i++]);
            if (str[i] == '\'')
                res = append_char(res, str[i++]);
        }
        else if (str[i] == '"')
        {
            res = append_char(res, str[i++]);
            while (str[i] && str[i] != '"')
            {
                if (str[i] == '$')
                {
                    // Si on a une concaténation comme "$"HOME → on veut garder le $
                    if (str[i + 1] == '"' && i > 0 && str[i - 1] == '"')
                        res = append_char(res, str[i++]); // ajoute juste le $
                    else
                        res = replace_variable_or_special(str, &i, res, envcp);
                }
                else
                    res = append_char(res, str[i++]);
            }
            if (str[i] == '"')
                res = append_char(res, str[i++]);
        }
        else if (str[i] == '$')
        {
            // Même logique en dehors de quotes
            if (str[i + 1] == '"' && i > 0 && str[i - 1] == '"')
                res = append_char(res, str[i++]);
            else
                res = replace_variable_or_special(str, &i, res, envcp);
        }
        else
        {
            res = append_char(res, str[i++]);
        }
    }
    return res;
}*/

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
		// printf("Processing char: '%c'\n", str[i]);
		// Cas \ pour ignorer $
		if (str[i] == '\\' && str[i + 1] == '$')
		{
			res = append_char(res, '$');
			i += 2;
			continue ;
		}
        // Gestion des quotes simples
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
            i++;
			continue ;
        }
        // Gestion des quotes doubles
        else if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            i++;
			continue ;
        }
        // Cas où $ doit être expansé
        // Si on est dans un heredoc, on évite l'expansion des variables
        else if (str[i] == '$' && !in_single_quote && !is_heredoc)
        {
            quoted = in_double_quote;
            res = replace_variable_or_special(str, &i, res, envcp, quoted);
        }
        else
        {
            res = append_char(res, str[i++]);
        }
    }
    printf("Final expanded string: '%s'\n", res);
    return res;
}

// Fonction pour remplacer toutes les variables d'environnement dans un tableau d'arguments
void expand_variables(char **args, char **envcp, int is_heredoc)
{
    int i = 0;
    int j = 0;
    char *tmp;
    char *res;
    char *new_args[1024]; // à adapter
    
    while (args[i])
    {
        tmp = replace_all_variables(args[i], envcp, is_heredoc);
        // printf("Expanded argument: %s\n", tmp);  // Afficher l'argument après expansion
        if (!tmp)
        {
            i++;
            continue; // skip cet argument
        }

        res = remove_quotes_or_slash(tmp);
        free(tmp);
        if (!res)
        {
            i++;
            continue;
        }

        if (res[0] == '\0' &&
            !strchr(args[i], '"') &&
            !strchr(args[i], '\''))
        {
            free(res);
            i++;
            continue;
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
        args[i] = ft_strdup(new_args[i]); // nouvelle copie pour éviter ownership multiple
        free(new_args[i]);
        i++;
    }
    args[i] = NULL;
}
