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
/*
// Gère les cas spéciaux après un '$' : $? ou $VAR_NAME
char *replace_variable_or_special(char *str, int *i, char *res)
{
    char var_name[256];
    int j;
    char *tmp;
    char *status;
    char *env_value;

    j = 0;
    (*i)++;
    if (!str[*i])
        return (append_char(res, '$'));
    if (str[*i] == '?')
    {
        // Si on rencontre $? (code de retour)
        status = ft_itoa(g_exit_status);
        if (!status)
        {
            free(res);
            return NULL;
        }
        tmp = ft_strjoin(res, status);
        free(res);
        free(status);
        res = tmp;
        (*i)++; // Sauter le '?'        
    }
    // Si c'est un guillemet juste après $, ce n'est pas une variable
    if (str[*i] == '"' || str[*i] == '\'')
    {
        res = append_char(res, '$');
        return(res);
    }
    // variable classique ($VAR_NAME)
    if (ft_isalpha(str[*i]) || str[*i] == '_')
    {
        // Si on rencontre une variable d'environnement
        while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
            var_name[j++] = str[(*i)++];
        var_name[j] = '\0';

        env_value = getenv(var_name);
        if (!env_value)
            env_value = "";  // Si la variable n'existe pas, utiliser une chaîne vide
        tmp = ft_strjoin(res, env_value);
        free(res);
        res = tmp;
    }
    else
    {
        // Cas d'un $ seul ou suivi d'un caractère invalide : on garde juste le '$'
        res = append_char(res, '$');
        // Si un autre caractère suit le $, on l'ajoute à la chaîne
        if (str[*i])
        {
            res = append_char(res, str[*i]);
            (*i)++;
        }
    }
    return (res);
}
// Fonction principale avec gestion des quotes
char *replace_all_variables(char *str)
{
    int i;
    char *res;

    i = 0;
    res = ft_strdup("");
    if (!res || !str)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '\'')
        {
            i++;
            while (str[i] && str[i] != '\'')
            {
                res = append_char(res, str[i]);
                i++;
            }
            if (str[i] == '\'')
                i++;
        }
        else if (str[i] == '"')
        {
            i++;
            while (str[i] && str[i] != '"')
            {
                if (str[i] == '$')
                    res = replace_variable_or_special(str, &i, res);
                else
                {
                    res = append_char(res, str[i]);
                    i++;
                }
            }
            if (str[i] == '"')
                i++;
        }
        else if (str[i] == '$')
            res = replace_variable_or_special(str, &i, res);
        else
        {
            res = append_char(res, str[i]);
            i++;
        }
    }
    return (res);
}*/

char *replace_variable_or_special(char *str, int *i, char *res)
{
    char var_name[256];
    int j = 0;
    char *tmp;
    char *status;
    char *env_value;

    (*i)++; // Skip the $

    // Fin de chaîne : juste un $
    if (!str[*i])
        return append_char(res, '$');

    // $? exit status
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
        //free(res);
        (*i)++;
        return (tmp);
    }

    // Si guillemet directement après $, ne pas interpréter
    if (str[*i] == '"' || str[*i] == '\'')
    {
        res = append_char(res, '$');
        return (res);
    }

    // Sinon, lire un vrai nom de variable
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
    else
    {
        // Cas spécial : $ suivi d'un caractère non valide
        res = append_char(res, '$');
        if (str[*i])
        {
            res = append_char(res, str[*i]);
            (*i)++;
        }
        return (res);
    }
}

char *replace_all_variables(char *str)
{
    int i = 0;
    char *res = ft_strdup("");
    if (!res || !str)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '\'') // quote simple : tout est littéral
        {
            res = append_char(res, str[i++]); // ajoute '
            while (str[i] && str[i] != '\'')
                res = append_char(res, str[i++]);
            if (str[i] == '\'') // ajoute la fin '
                res = append_char(res, str[i++]);
        }
        else if (str[i] == '"') // quote double : on autorise les $
        {
            res = append_char(res, str[i++]); // ajoute "
            while (str[i] && str[i] != '"')
            {
                if (str[i] == '$')
                {
                    // Ne pas interpréter $ suivi de quote
                    if (str[i + 1] == '"' || str[i + 1] == '\'')
                    {
                        res = append_char(res, '$');
                        i++;
                    }
                    else
                        res = replace_variable_or_special(str, &i, res);
                }
                else
                    res = append_char(res, str[i++]);
            }
            if (str[i] == '"')
                res = append_char(res, str[i++]); // ajoute "
        }
        else if (str[i] == '$')
        {
            // Si $ est suivi d’un quote, ne pas interpréter
            if (str[i + 1] == '"' || str[i + 1] == '\'')
            {
                res = append_char(res, '$');
                i++;
            }
            else
                res = replace_variable_or_special(str, &i, res);
        }
        else
            res = append_char(res, str[i++]);
    }
    return (res);
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
