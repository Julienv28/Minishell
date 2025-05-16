#include "../includes/minishell.h"

// Ajoute un caractère simple à la chaîne résultante
char *append_char(char *res, char c)
{
    char current[2];
    char *tmp;

    current[0] = c;
    current[1] = '\0';
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

    // Choix entre get_env_value ou get_value_cleaned selon le flag quoted
    if (quoted)
        env_value = get_env_value(var_name, envcp); // On garde les espaces dans le cas de la citation
    else
        env_value = get_value_cleaned(var_name, envcp); // On nettoie les espaces si non cité

    if (!env_value)
    {
        env_value = ""; // En cas d'absence de variable
    }

    tmp = ft_strjoin(res, env_value); // Concaténation avec le résultat
    free(res);
    return tmp;
}

/*
char *expand_single_quote_literal(char *str, int *i, char *res)
{
    (*i)++; // skip first '
    while (str[*i] && str[*i] != '\'')
        res = append_char(res, str[(*i)++]);
    if (str[*i] == '\'')
        (*i)++;
    return res;
}

char *expand_gettext_style(char *str, int *i, char *res)
{
    (*i)++; // skip initial "
    while (str[*i] && str[*i] != '"')
        res = append_char(res, str[(*i)++]);
    if (str[*i] == '"')
        (*i)++;
    return res;
}*/

char *replace_variable_or_special(char *str, int *i, char *res, char **envcp, int quoted)
{
    (*i)++; // skip $

    if (!str[*i])
        return append_char(res, '$');

    // Gestion de $'' ou $"" (quote ANSI C ou gettext)
    if ((str[*i] == '\'' || str[*i] == '"') && *i == 1)
    {
        char quote = str[*i];
        (*i)++; // skip opening quote

        while (str[*i] && str[*i] != quote)
            res = append_char(res, str[(*i)++]);

        if (str[*i] == quote)
            (*i)++; // skip closing quote

        return res; // PAS de $ ajouté
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

    if (!ft_isalpha(str[*i]) && str[*i] != '_' && !ft_isdigit(str[*i]))
        return append_char(res, '$');

    if (ft_isdigit(str[*i]))
    {
        (*i)++;
        return res;
    }

    return expand_env_variable(str, i, res, envcp, quoted);
}

/*
char *replace_variable_or_special(char *str, int *i, char *res, char **envcp)
{
    char var_name[256];
    int j = 0;
    char *tmp;
    char *status;
    char *env_value;

    (*i)++; // Skip the $

    // Cas spécial : $ suivi de quote simple (string literal)
    if (str[*i] == '\'')
    {
        (*i)++; // skip 1er quote (')
        while (str[*i])
        {
            if (str[*i] == '\'')
            {
                (*i)++; // skip quote fermante (')
                // check si une autre quote suit pour cas $'HO''ME'
                if (str[*i] == '\'')
                    continue;
                else
                    break;
            }
            res = append_char(res, str[(*i)++]);
        }
        return res;
    }

    // Gestion gettext-style : $"..."
    // Si $ est suivi directement de " → gettext style ($"HOME") → remplace par contenu
    if (str[*i] == '"' && *i == 1) // On n'est pas dans une quote concaténée, genre "$"HOME
    {
        (*i)++; // skip le "
        while (str[*i] && str[*i] != '"')
            res = append_char(res, str[(*i)++]);
        if (str[*i] == '"')
            (*i)++; // skip le " fermant
        return res;
    }

    // Fin de chaîne ou $ tout seul
    if (!str[*i])
        return append_char(res, '$');

    // Cas : $ suivi d'un caractère non valide (pas une var)
    if (!ft_isalpha(str[*i]) && str[*i] != '_' && str[*i] != '?' && !ft_isdigit(str[*i]))
    {
        res = append_char(res, '$');
        return res;
    }

    // Cas spécial : $?
    // if (str[*i] == '?' && (str[*i + 1] == '\0' || str[*i + 1] == ' '))
    if (str[*i] == '?')
    {
        status = ft_itoa(g_exit_status);
        if (!status)
        {
            free(res);
            return NULL;
        }
        tmp = ft_strjoin(res, status);
        free(res);
        free(status);
        (*i)++;
        return tmp;
    }
    // Cas : $ suivi d'un chiffre ($1, $9...)
    if (ft_isdigit(str[*i]))
    {
        (*i)++;
        return res; // rien à ajouter
    }

    // Cas : variable standard ($HOME, etc.)
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        var_name[j++] = str[(*i)++];
    var_name[j] = '\0';

    // env_value = getenv(var_name);
    env_value = get_value_cleaned(var_name, envcp);
    if (!env_value)
    {
        if (ft_strcmp(var_name, "UID") == 0)
            ft_putstr_fd("UID non défini dans l'environnement\n", STDERR_FILENO);
        env_value = "";
    }

    // enlever espace indésirable

    tmp = ft_strjoin(res, env_value);
    if (!tmp)
    {
        free(res);
        return NULL;
    }
    free(res);
    return tmp;
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

char *replace_all_variables(char *str, char **envcp)
{
    int i = 0;
    char *res = ft_strdup("");
    int in_single_quote = 0;
    int in_double_quote = 0;
    int quoted = 0;

    if (!res || !str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            res = append_char(res, str[i++]);
            // i++; // skip quote
        }
        else if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            res = append_char(res, str[i++]);
            ;
            // i++; // skip quote
        }
        else if (str[i] == '$' && !in_single_quote)
        {
            quoted = in_double_quote;
            res = replace_variable_or_special(str, &i, res, envcp, quoted);
        }
        else
        {
            res = append_char(res, str[i++]);
        }
    }
    return res;
}

// Fonction pour remplacer toutes les variables d'environnement dans un tableau d'arguments
void expand_variables(char **args, char **envcp)
{
    int i = 0;
    int j = 0;
    char *tmp;
    char *res;
    char *new_args[1024]; // à adapter

    while (args[i])
    {
        tmp = replace_all_variables(args[i], envcp);
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
