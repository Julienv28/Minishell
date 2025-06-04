#include "../includes/minishell.h"

// CRÉATION ET GESTION DES TOKENS
t_token	*add_token(t_token **head, char *str, int type)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->value = ft_strdup(str);
	new->type = type;
	new->next = NULL;
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
    int expect_cmd;
    t_token *tokens;
    int redirection_status;

    i = 0;
    tokens = NULL;
    expect_cmd = 1;
    while ((*str)[i])
    {
        while ((*str)[i] == ' ')
            i++;
        if (!(*str)[i])
            break ;
        if ((*str)[i] == '|')
        {
            if (check_pipe(*str, i) == -1)
            {
                free_tokens(tokens);
                return (NULL);
            }
            add_token(&tokens, "|", PIPE);
            i++;
            expect_cmd = 1;
        }
        else if ((*str)[i] == '&' || (*str)[i] == ':' || (*str)[i] == '!' || (*str)[i] == '#' || (*str)[i] == ';')
        {
            if (check_input(*str, i) == -1)
                return (NULL);
        }
        else
        {
            redirection_status = handle_redirection(*str, &i, &tokens, envcp);
            if (redirection_status == -1)
            {
                free_tokens(tokens);
                return (NULL);
            }
            if (redirection_status == 1)
                continue;
            if (handle_word(str, &i, &tokens, &expect_cmd) == -1)
                break ;
        }
    }
    return (tokens);
}
