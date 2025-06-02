#include "../includes/minishell.h"

// CRÉATION ET GESTION DES TOKENS
t_token	*add_token(t_token **head, char *str, int type)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->value = ft_strdup(str); // Copie la valeur du token
	new->type = type;            // Lui assigne un type
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

// int	handle_token_type(char **str, int *i, t_token **tokens, char **envcp)
// {
// 	int	status;
// 	status = handle_redirection(*str, i, tokens, envcp);
// 	if (status == -1)
// 		return (-1);
// 	if (status == 1)
// 		return (0);
// 	return (0);
// }

// int	handle_special_char(char **str, int *i, t_token **tokens, int *expect_cmd)
// {
// 	if ((*str)[*i] == '|')
// 	{
// 		if (check_pipe(*str, *i) == -1)
// 			return (-1);
// 		add_token(tokens, "|", PIPE);
// 		(*i)++;
// 		*expect_cmd = 1;
// 		return (1);
// 	}
// 	if ((*str)[*i] == '&' || (*str)[*i] == ':'
// 		|| (*str)[*i] == '!' || (*str)[*i] == '#'
// 		|| (*str)[*i] == ';')
// 		return (check_input(*str, *i) == -1 ? -1 : 1);
// 	return (0);
// }

// Analyser la ligne de commande et créer des tokens
// t_token	*create_tokens(char **str, char **envcp)
// {
// 	t_token	*tokens;
// 	int		i;
// 	int		expect_cmd;
// 	int		status;

// 	tokens = NULL;
// 	i = 0;
// 	expect_cmd = 1;
// 	while ((*str)[i])
// 	{
// 		while ((*str)[i] == ' ')
// 			i++;
// 		if (!(*str)[i])
// 			break ;
// 		status = handle_special_char(str, &i, &tokens, &expect_cmd);
// 		if (status == -1)
// 			return (free_tokens(tokens), NULL);
// 		if (status == 1) // Pertinent ici ? Parce que de toute facon on a 1 et -1, nothing else.
// 			continue ;
// 		status = handle_token_type(str, &i, &tokens, envcp);
// 		if (status == -1)
// 			return (free_tokens(tokens), NULL);
// 		if (handle_word(str, &i, &tokens, &expect_cmd) == -1)
// 			return (free_tokens(tokens), NULL);
// 	}
// 	return (tokens);
// }
	/*tokens = NULL;
	i = 0;
	expect_cmd = 1;
	while ((*str)[i])
	{
		while ((*str)[i] == ' ')
			i++;
		if (!(*str)[i])
			break ;
		status = handle_special_char(str, &i, &tokens, &expect_cmd);
		if (status == -1)
			return (free_tokens(tokens), NULL);
		if (status == 1) // Pertinent ici ? Parce que de toute facon on a 1 et -1, nothing else.
			continue ;
		status = handle_token_type(str, &i, &tokens, envcp);
		if (status == -1)
			return (free_tokens(tokens), NULL);
		if (handle_word(str, &i, &tokens, &expect_cmd) == -1)
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}*/


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
        else
        {
            redirection_status = handle_redirection(*str, &i, &tokens, envcp);
            if (redirection_status == -1)
            {
                free_tokens(tokens);
                return (NULL);
            }
            if (redirection_status == 1)
                continue; // Redirection traitée, boucle suivante
            if (handle_word(str, &i, &tokens, &expect_cmd) == -1)
                break;
        }
    }
    return (tokens);
}
