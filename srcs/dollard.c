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

char *replace_variable_or_special(char *str, int *i, char *res)
{
    char var_name[256];
    int j = 0;
    char *tmp;
    char *status;
    char *env_value;
    char    quote;

    (*i)++; // Skip the $

    // Fin de chaîne : juste un $
    if (!str[*i])
        return (append_char(res, '$'));

    //Si $ est suivi de ' ou ", c'est une chaîne spéciale : on ignore le $
    if (str[*i] == '\'' || str[*i] == '"')
    {
        quote = str[*i];
        (*i)++; // skip la quote
        while (str[*i] && str[*i] != quote)
            res = append_char(res, str[(*i)++]);
        if (str[*i] == quote)
            (*i)++; // skip la quote fermante
        return (res);
    }
    if (str[*i] == '"' || str[*i] == '\'')
    {
        res = append_char(res, '$');
        return res;
    }

    // Cas : caractère non valide après $
    // Si ce n'est pas une lettre, underscore, chiffre ou ?, on garde le $
    if (!str[*i] || (!ft_isalpha(str[*i]) && str[*i] != '_' && str[*i] != '?' && !ft_isdigit(str[*i])))
    {
        res = append_char(res, '$');
        return res;
    }

    // Cas special $? exit status
    if (str[*i] == '?')
    {
        status = ft_itoa(g_exit_status);
        if (!status)
        {
            free(res);
            return (NULL);
        }
        tmp = ft_strjoin(res, status);
        free(status);
        (*i)++;
        return (tmp);
    }

    // Cas special: $ suivi d'un chiffre (ex: $9)
    if (ft_isdigit(str[*i]))
    {
        (*i)++; // skip la quote
        tmp = ft_strjoin(res, ""); // rien à ajouter
        free(res);
        return (tmp);
    }

    // Cas nom de variable valide
    if (ft_isalpha(str[*i]) || str[*i] == '_')
    {
        while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        {
            var_name[j++] = str[*i];
            (*i)++;
        }
        var_name[j] = '\0';
        env_value = getenv(var_name);
        if (!env_value)
            env_value = "";
        tmp = ft_strjoin(res, env_value);
        free(res);
        return (tmp);
    }

    // Pardefault garder le $
    res = append_char(res, '$');
    if (str[*i])
    {
        res = append_char(res, str[*i]);
        (*i)++;
    }
    return (res);
}

char *replace_all_variables(char *str)
{
    int i = 0;
    char *res = ft_strdup("");
    if (!res || !str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'') // quotes simples : tout est littéral
        {
            res = append_char(res, str[i++]);
            while (str[i] && str[i] != '\'')
                res = append_char(res, str[i++]);
            if (str[i] == '\'')
                res = append_char(res, str[i++]);
        }
        else if (str[i] == '"') // quotes doubles : on autorise $
        {
            res = append_char(res, str[i++]);
            while (str[i] && str[i] != '"')
            {
                if (str[i] == '$')
                    res = replace_variable_or_special(str, &i, res);
                else
                    res = append_char(res, str[i++]);
            }
            if (str[i] == '"')
                res = append_char(res, str[i++]);
        }
        else if (str[i] == '$')
        {
            res = replace_variable_or_special(str, &i, res);
        }
        else
            res = append_char(res, str[i++]);
    }
    return res;
}


// Fonction pour remplacer toutes les variables d'environnement dans un tableau d'arguments
void replace_exit_and_env_status(char **args)
{
    int i;
    char *tmp;
    char    *res;

    if (!args)
        return;
    i = 0;
    while (args[i])
    {
        tmp = replace_all_variables(args[i]);  // Remplacer les valeurs des variables d’environnement
        if (!tmp)
            return;
        res = remove_quotes_or_slash(tmp); // // Ensuite, on retire les quotes et backslashes
        free(tmp);
        free(args[i]);
        args[i] = res;
        i++;
    }
}
