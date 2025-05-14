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
    if (str[*i] == '?' && (str[*i + 1] == '\0' || str[*i + 1] == ' '))
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
}

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

    if (!res || !str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            i++; // skip quote
        }
        else if (str[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            i++; // skip quote
        }
        else if (str[i] == '$' && !in_single_quote)
        {
            res = replace_variable_or_special(str, &i, res, envcp);
        }
        else
        {
            res = append_char(res, str[i++]);
        }
    }
    return res;
}

// Fonction pour remplacer toutes les variables d'environnement dans un tableau d'arguments
void replace_exit_and_env_status(char **args, char **envcp)
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
