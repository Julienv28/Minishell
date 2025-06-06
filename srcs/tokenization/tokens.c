#include "../includes/minishell.h"
/*
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
}*/

t_token	*add_token(t_token **head, char *str, int type, int is_quoted)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(str);
	new->type = type;
	new->is_quoted = is_quoted;
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

int skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
    if (!str[*i])
        return (-1);
    return (0);
}

int	process_pipe(char *str, int *i, t_token **tokens, int *expect_cmd)
{
	if (str[*i] == '|')
	{
		if (check_pipe(str, *i) == -1)
		{
			free_tokens(*tokens);
			return (-1);
		}
		add_token(tokens, "|", PIPE, 0);
		(*i)++;
		*expect_cmd = 1;
		return (1);
	}
	return (0);
}

int	process_special_chars(char *str, int i)
{
	if (str[i] == '&' || str[i] == ':' 
        || str[i] == '!' || str[i] == '#' || str[i] == ';')
	{
		if (check_input(str, i) == -1)
			return (-1);
	}
	return (0);
}

int process_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
    int redirection_status;

    redirection_status = handle_redirection(str, i, tokens, envcp);
    if (redirection_status == -1)
    {
        free_tokens(*tokens);
        return (-1);
    }
    return (redirection_status);
}

int process_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
	if (handle_word(str, i, tokens, expect_cmd) == -1)
		return (-1);
	return (0);
}

t_token	*process_token_loop(char *str, char **envcp)
{
	int		i;
	// int		expect_cmd;
    int     tab[2];
	t_token	*tokens;
	// int		ret;

	i = 0;
	tab[0] = 1; //expect_cmd = 1;
	tokens = NULL;
	while (str[i])
	{
		if (skip_spaces(str, &i) == -1)
			break ;
        tab[1] = process_pipe(str, &i, &tokens, &tab[0]);
		if (tab[1] == -1 || process_special_chars(str, i) == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
        tab[1] = process_redirection(str, &i, &tokens, envcp);
		if (tab[1] == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
		if (process_word(&str, &i, &tokens, &tab[0]) == -1)
			break ;
	}
	return (tokens);
}

t_token	*create_tokens(char **str, char **envcp)
{
	t_token	*tokens;

	tokens = process_token_loop(*str, envcp);
	return (tokens);
}


/*
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
}*/
