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

    // printf("Ajout token \"%s\" -> type: %d\n", str, type);
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
t_token *create_tokens(char **str, char **envcp)
{
    int i;
    int expect_cmd; // Flag pour savoir si on attend une CMD (1) ou un ARG
    t_token *tokens;
    int redirection_status;

    i = 0;
    tokens = NULL;
    expect_cmd = 1; // On attend une CMD en premier

    while ((*str)[i])
    {
        while ((*str)[i] == ' ') // Sauter les espaces
            i++;

        if (!(*str)[i]) // Fin de la chaîne
            break;

        // Détection des pipes
        if ((*str)[i] == '|')
        {
            if (check_pipe(*str, i) == -1)
                return (NULL);
            add_token(&tokens, "|", PIPE);
            i++;
            expect_cmd = 1; // Après un pipe, on attend une nouvelle commande
        }
        // Gestion des autres opérateurs si nécessaire
        else if ((*str)[i] == '&' || (*str)[i] == ':' || (*str)[i] == '!' || (*str)[i] == '#' || (*str)[i] == ';')
        {
            if (check_input(*str, i) == -1)
                return (NULL);
        }
        // Gestion de la redirection
        else
        {
            redirection_status = handle_redirection(*str, &i, &tokens, envcp);
            if (redirection_status == -1)
            {
                free_tokens(tokens);
                return (NULL);
            }
            if (redirection_status == 1)
            {
                // expect_cmd = 0; // Après redirection, on attend pas spécialement une CMD
                continue; // Redirection traitée, boucle suivante
            }
            // Après avoir traité la redirection, on devrait ajouter la commande suivante
            // La fonction handle_word permet de gérer correctement les commandes et leurs arguments
            if (handle_word(str, &i, &tokens, &expect_cmd) == -1)
                break;
        }
    }

    return (tokens);
}
