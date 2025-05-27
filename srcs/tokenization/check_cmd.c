#include "../includes/minishell.h"

int check_redirection(char *str, int *i)
{
    while (str[*i] == ' ')
        (*i)++;
    if (!str[*i])
    {
        printf("Erreur : syntax error near unexpected token `newline'\n");
        return (-1);
    }
    // Si on a une redirection qui est suivie d'un autre token spécial
    if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
    {
        if ((str[*i] == '>' && str[*i + 1] == '>') || (str[*i] == '<' && str[*i + 1] == '<'))
            printf("Erreur : syntax error near unexpected token `%c%c'\n", str[*i], str[*i]);
        else if (str[*i] == '>' && str[*i - 1] == '<') // cas <>
            printf("Erreur : syntax error near unexpected token `newline'\n");
        else
            printf("Erreur : syntax error near unexpected token `%c'\n", str[*i]);
        return (-1);
    }
    return (0);
}

int check_pipe(char *str, int i)
{
    int j;

    // Vérifie si on a "||"
    if (str[i] == '|' && str[i + 1] == '|')
    {
        printf("Erreur : syntax error near unexpected token `||'\n");
        return -1;
    }
    // Verif pipe au debut de commande
    if (i == 0)
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
        return (-1);
    }

    // Verif pipe en fin de commande
    j = i + 1;
    while (str[j] && str[j] == ' ')
        j++;
    if (str[j] == '\0')
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[j]);
        return (-1);
    }
    // Verif double pipe sans commande entre
    if (str[j] == '|')
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[j]);
        return (-1);
    }
    return (0);
}

int check_input(char *str, int i)
{
    // Vérifier si le caractère précédent est aussi un pipe
    if ((str[i] == '&' && str[i + 1] == '&') || (str[i] == ';' && str[i + 1] == ';'))
    {
        printf("Erreur : syntax error near unexpected token `%c%c'\n", str[i], str[i + 1]);
        return (-1);
    }
    else if (str[i] == ':' || str[i] == '!' || str[i] == '#')
        return (-1);
    else
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
        return (-1);
    }
    return (0);
}
