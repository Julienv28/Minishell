#include "../includes/minishell.h"

// CRÉATION ET GESTION DES TOKENS

// Ajouter un token à la liste
t_token *add_token(t_token **head, char *str, int type)
{
    t_token *new;
    t_token *tmp;

    new = malloc(sizeof(t_token)); // Créer un new token
    if (new == NULL)
        return (NULL);
    new->value = ft_strdup(str); // Copie la valeur du token
    new->type = type;            // Lui assigne un type
    new->next = NULL;

    printf("Ajout token \"%s\" -> type: %d\n", str, type);
    if (!*head)
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    return (new);
}

// Analyser la ligne de commande et créer des tokens
t_token *create_tokens(char **str)
{
    int i;
    int expect_cmd; // Flag pour savoir si on attend une CMD (1) ou un ARG
    t_token *tokens;
    int redirection_status;

    i = 0;
    tokens = NULL;
    expect_cmd = 1; // 1 car on attend une CMD en 1er
    while ((*str)[i])
    {
        while ((*str)[i] == ' ')
            i++;
        if (!(*str)[i])
            break;

        if ((*str)[i] == '|')
        {
            if (check_pipe(*str, i) == -1)
                return (NULL);
            add_token(&tokens, "|", PIPE);
            printf("Détection du pipe '|\n");
            i++;
            expect_cmd = 1;
        }
        else
        {
            redirection_status = handle_redirection(*str, &i, &tokens); // Ne modifie pas str, donc OK
            if (redirection_status == -1)
                return (NULL);
            //handle_redirection(*str, &i, &tokens); // Ne modifie pas str, donc OK
            handle_word(str, &i, &tokens, &expect_cmd);
        }
    }
    return (tokens);
}