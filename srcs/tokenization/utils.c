#include "../includes/minishell.h"

// Afficher les tokens
void print_tokens(t_token *tokens)
{
    int i;

    i = 0;
    while (tokens)
    {
        printf("Token[%d]: %s\n", i, tokens->value);
        i++;
        tokens = tokens->next;
    }
}

// Liberer les tokens
void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}
