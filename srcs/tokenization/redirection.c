#include "../includes/minishell.h"
/*
// Fonction pour gérer la redirection dans la commande
int handle_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
    int start;
    int type;
    char *word;
    char *symbol;
    char *expanded;
    char *cleaned;

    type = parse_redirection(str, i);
    if (type)
    {
        symbol = add_symbol(type);       // Créer le symbole de redirection
        add_token(tokens, symbol, type); // Ajouter le token de redirection
        free(symbol);
        if (check_redirection(str, i) == -1)
        {
            g_exit_status = 2;
            return (-1);
        }
        while (str[*i] == ' ')
            (*i)++;
        start = *i;
        while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' && str[*i] != '>')
            (*i)++;
        if (*i == start) // Si aucun fichier n'est spécifié
        {
            printf("Erreur : Redirection sans fichier spécifié !\n");
            return (-1);
        }
        word = ft_strndup(str + start, *i - start); // Copier le fichier de redirection
        if (type == HEREDOC && limiter_is_quoted(word) == 0)
            add_token(tokens, word, ARG); // Ajouter le fichier comme argument
        else
        {
            expanded = replace_all_variables(word, envcp, 0);
            cleaned = remove_quotes_or_slash(expanded);
            free(expanded);
            word = cleaned;
            add_token(tokens, word, ARG); // Ajouter le fichier comme argument
        }
        free(word);
        return (1); // Redirection traitée
    }
    return (0); // Pas de redirection
}*/
char	*extract_redir_word(char *str, int *i)
{
	int	start;

	while (str[*i] == ' ')
		(*i)++;
	start = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '|'
		&& str[*i] != '<' && str[*i] != '>')
		(*i)++;
	if (*i == start)
    {
        (*i)++;
		return (NULL);
    }
	return (ft_strndup(str + start, *i - start));
}

char	*expand_clean_word(char *word, char **envcp)
{
	char	*expanded;
	char	*cleaned;

	expanded = replace_all_variables(word, envcp, 0);
	cleaned = remove_quotes_or_slash(expanded);
	free(expanded);
	return (cleaned);
}

int	handle_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
	int		type;
	char	*symbol;
	char	*word;

	type = parse_redirection(str, i);
	if (!type)
		return (0);
	symbol = add_symbol(type);
	add_token(tokens, symbol, type);
	free(symbol);
	if (check_redirection(str, i) == -1)
		return (g_exit_status = 2, -1);
	word = extract_redir_word(str, i);
	if (!word)
		return (printf("Erreur : Redirection sans fichier !\n"), -1);
	if (type == HEREDOC && limiter_is_quoted(word) == 0)
    {
		add_token(tokens, word, ARG);
        free(word);
    }
	else
	{
		char *expanded = expand_clean_word(word, envcp);
		add_token(tokens, word, ARG);
        free(expanded);
        free(word);
	}
	return (1);
}

// Analyser les redirections et avancer l'index
int parse_redirection(char *str, int *i)
{
    if (str[*i] == '2' && str[*i + 1] == '>' && str[*i + 2] == '>')
    {
        (*i) += 3;
        return (ERR_REDIR);
    }
    else if (str[*i] == '2' && str[*i + 1] == '>')
    {
        (*i) += 2;
        return (ERR_REDIR);
    }
    if (str[*i] == '<' && str[*i + 1] == '<')
    {
        (*i) += 2; // Avancer l'index de 2 pour ignorer "<<"
        return (HEREDOC);
    }
    else if (str[*i] == '>' && str[*i + 1] == '>')
    {
        (*i) += 2;
        return (APPEND);
    }
    else if (str[*i] == '<')
    {
        (*i)++;
        return (INPUT);
    }
    else if (str[*i] == '>')
    {
        (*i)++;
        return (TRUNC);
    }
    return (0);
}

// Convertir un type en symbole de redirection
char	*add_symbol(int type)
{
	if (type == HEREDOC)
		return (ft_strdup("<<"));
	else if (type == APPEND)
		return (ft_strdup(">>"));
	else if (type == INPUT)
		return (ft_strdup("<"));
	else if (type == TRUNC)
		return (ft_strdup(">"));
	else if (type == ERR_REDIR)
		return (ft_strdup("2>"));
	return (NULL);
}

int	ft_redirect_in(char *file, int *fd_in)
{
	int	fd;

	fd = open_file_cmd(file);
	if (fd < 0)
		return (perror(file), -1);
	if (fd_in)
		*fd_in = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int ft_redirect_out(char *file, int flag, int *fd_out)
{
    int fd;

    fd = open_outfile(file, flag);
    if (fd < 0)
        return (perror(file), -1);
    if (fd_out)
        *fd_out = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

int ft_redirection_err(t_com_list *cmd, int *fd_err)
{
    int fd;

    if (!cmd->errfile)
        return (0);
    fd = open_errfile(cmd->errfile);
    if (fd < 0)
        return (perror(cmd->errfile), -1);
    if (fd_err)
        *fd_err = dup(STDERR_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
    return (0);
}

int ft_redirection(t_com_list *cmd, int *fd_in, int *fd_out, int *fd_err)
{
    t_file_list *tmp;
    int fd;

    if (cmd->heredoc_fd > 0)
    {
        if (fd_in)
            *fd_in = dup(STDIN_FILENO);
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
    }
    if (cmd->infile && ft_redirect_in(cmd->infile, fd_in) < 0)
        return (-1);
    if (cmd->outfile && ft_redirect_out(cmd->outfile,
                                        cmd->flag_out, fd_out) < 0)
        return (-1);
    tmp = cmd->all_outfilles;
    while (tmp)
    {
        fd = open_outfile(tmp->filename, tmp->flag);
        if (fd < 0)
            return (perror(tmp->filename), -1);
        close(fd);
        tmp = tmp->next;
    }
    return (ft_redirection_err(cmd, fd_err));
}
/*
int ft_redirection(t_com_list *command, int *mem_fd_in, int *mem_fd_out, int *mem_fd_err)
{
    int fd;
    t_file_list *tmp;

    if (command->heredoc_fd > 0)
    {
        if (mem_fd_in)
            *mem_fd_in = dup(STDIN_FILENO);
        dup2(command->heredoc_fd, STDIN_FILENO);
        close(command->heredoc_fd);
    }
    if (command->infile)
    {
        fd = open_file_cmd(command->infile);
        if (fd < 0)
        {
            perror(command->infile);
            return (-1);
        }
        if (mem_fd_in)
            *mem_fd_in = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (command->outfile)
    {
        if (mem_fd_out)
            *mem_fd_out = dup(STDOUT_FILENO);
        fd = open_outfile(command->outfile, command->flag_out);
        if (fd < 0)
        {
            perror(command->outfile);
            return (-1); // Ne pas créer quoi que ce soit d’autre
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    tmp = command->all_outfilles;
    while (tmp)
    {
        fd = open_outfile(tmp->filename, tmp->flag);
        if (fd < 0)
        {
            perror(tmp->filename);
            return (-1);
        }
        close(fd); // just to create it
        tmp = tmp->next;
    }


    if (command->errfile)
    {
        if (mem_fd_err)
            *mem_fd_err = dup(STDERR_FILENO);
        fd = open_errfile(command->errfile);
        if (fd < 0)
        {
            perror(command->errfile);
            return (-1);
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
    }
    return (0);
}*/

void	restore_redirections(int mem_fd_in, int mem_fd_out, int mem_fd_err)
{
	if (mem_fd_in != -1)
	{
		dup2(mem_fd_in, STDIN_FILENO);
		close(mem_fd_in);
	}
	if (mem_fd_out != -1)
	{
		dup2(mem_fd_out, STDOUT_FILENO);
		close(mem_fd_out);
	}
	if (mem_fd_err != -1)
	{
		dup2(mem_fd_err, STDERR_FILENO);
		close(mem_fd_err);
	}
}
