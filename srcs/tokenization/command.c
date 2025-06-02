/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:08:55 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/02 11:00:09 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ANALYSE DES COMMANDES

// Concaténer deux chaînes de caractères (une CMD et un nouvel ARG)
char	*concat_command(char *current_command, char *new_part)
{
	char	*new_command;
	int		len_current;
	int		len_new;

	if (current_command)
		len_current = ft_strlen(current_command);
	else
		len_current = 0;
	len_new = ft_strlen(new_part);
	new_command = malloc(len_current + len_new + 2); // +2 pour l'espace et '\0'
	if (new_command == NULL)
		return (NULL);
    // Copier l'ancienne commande si elle existe
	if (current_command)
	{
		ft_strcpy(new_command, current_command);
		ft_strcat(new_command, " "); // Ajoute un espace entre les commandes
	}
	else
		new_command[0] = '\0'; // Initialiser à une chaîne vide si `current_command` est NULL
	ft_strcat(new_command, new_part); // Ajouter le nouvel element
	free(current_command);
	return (new_command);
}

void	free_file_list(t_file_list *list)
{
	t_file_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}

int	limiter_is_quoted(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (1);
	return (0);
}
/*
int handle_heredoc(char *limiter,char **envcp)
{
    int pipefd[2];
    char *line;
    char    *expanded;
    int is_heredoc = 0;
    char    *cleaned_limiter;

    // Enlever les guillemets autour du limiter pour la comparaison
    cleaned_limiter = remove_quotes_or_slash(limiter);
    // Vérifier si le limiter est entre guillemets
    if (limiter_is_quoted(limiter) == 0)
        is_heredoc = 1;  // Pas d'expansion des variables si entre guillemets
    else
        is_heredoc = 0;  // Expansion des variables autorisée
    printf("limiteur quote = %s et heredoc = %d\n", limiter, is_heredoc);
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    while(1)
    {
        line = readline("> ");
        printf("DEBUG: readline retourné: \"%s\"\n", line);
        printf("DEBUG: line=\"%s\" (%zu chars), limiter=\"%s\" (%zu chars)\n",
    line, ft_strlen(line),
    limiter, ft_strlen(limiter));

        if (!line || ft_strcmp(line, cleaned_limiter) == 0)
        {
            free(line);
            break;
        }
        printf("before expanded = %s\n", expanded);
        expanded = replace_all_variables(line, envcp, is_heredoc);
        printf("after expanded = %s\n", expanded);
        write(pipefd[1], expanded, ft_strlen(expanded));
        write(pipefd[1], "\n", 1);
        free(expanded);
        free(line);
    }
    close(pipefd[1]);
    return (pipefd[0]);
}*/

int handle_heredoc(char *limiter, char **envcp)
{
    int pipefd[2];
    char *line = NULL;
    char *expanded_line = NULL;
    char *expanded_limiter = NULL;
    char *cleaned_limiter = NULL;
    int is_heredoc;

    // Expansion du limiter (si pas entre quotes)
    if (limiter_is_quoted(limiter))
        is_heredoc = 1;  // quoted → pas d'expansion
    else
        is_heredoc = 0;  // pas quoted → expansion des variables

    // Expand + clean le limiter
    expanded_limiter = replace_all_variables(limiter, envcp, is_heredoc);  // is_heredoc = 0 pour expansion complète
    cleaned_limiter = remove_quotes_or_slash(expanded_limiter);
    if (!cleaned_limiter)
    cleaned_limiter = ft_strdup("");
    free(expanded_limiter);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        free(cleaned_limiter);
        return -1;
    }

    printf("detection HEREDOC\n");
    printf("filename = %s\n", cleaned_limiter);
    printf("limiteur quote = %s et heredoc = %d\n", limiter, is_heredoc);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        printf("DEBUG: readline retourné: \"%s\"\n", line);
        printf("DEBUG: line=\"%s\" (%zu chars), limiter=\"%s\" (%zu chars)\n",
               line, ft_strlen(line),
               cleaned_limiter, ft_strlen(cleaned_limiter));
        // Fin du heredoc ?
        if (ft_strcmp(line, cleaned_limiter) == 0)
        {
            free(line);
            break;
        }
        expanded_line = replace_all_variables(line, envcp, is_heredoc);
        printf("before expanded = %s\n", line);
        printf("after expanded = %s\n", expanded_line);

        write(pipefd[1], expanded_line, ft_strlen(expanded_line));
        write(pipefd[1], "\n", 1);

        free(line);
        free(expanded_line);
    }
    free(cleaned_limiter);
    close(pipefd[1]);
    return pipefd[0];
}

t_com_list	*tokens_to_cmds(t_token *tokens, char **envcp)
{
    t_com_list *cmd_list = NULL;
    t_com_list *current_cmd = NULL;
    t_token *tmp = tokens;
    t_com_list *new_cmd;
    char *filename;
    char *pending_outfile = NULL;
    int pending_flag_out = -1;
    char *pending_errfile = NULL;
    char *pending_infile = NULL;
    int pending_flag_in = -1;
    int flag;
    int redir_type;
    t_file_list *pending_all_outfiles = NULL;
    int fd;

	while (tmp)
	{
		// Affiche la valeur du token pour débogage
		if (tmp->type == CMD)
        {
            if (!tmp->value)
            {
                fprintf(stderr, "Erreur : token CMD avec valeur NULL\n");
                tmp = tmp->next;
                continue;
            }
            char *expanded = replace_all_variables(tmp->value, envcp, 0);
            new_cmd = list_new(expanded);
            new_cmd->args = malloc(sizeof(char*) * MAX_ARGS);
            if (!new_cmd->args)
                return (NULL);
            new_cmd->args[0] = ft_strdup(expanded);
            new_cmd->args[1] = NULL;
            free(expanded);
            new_cmd->heredoc_fd = -1;
			if (pending_outfile)
			{
				new_cmd->outfile = pending_outfile;
				new_cmd->flag_out = pending_flag_out;
				add_outfile(&new_cmd->all_outfilles, pending_outfile, pending_flag_out);
				pending_outfile = NULL;
				pending_flag_out = -1;
			}
			if (pending_infile)
			{
				new_cmd->infile = pending_infile;
				new_cmd->flag_in = pending_flag_in;
				pending_infile = NULL;
				pending_flag_in = -1;
			}
			if (!cmd_list)
				cmd_list = new_cmd;
			else
				add_bottom(&cmd_list, new_cmd);
			current_cmd = new_cmd;
		}
		else if (tmp->type == ARG && current_cmd)
		{
			char	*expanded = replace_all_variables(tmp->value, envcp, 0);
            int     i = 0;
            while (current_cmd->args[i])
                i++;
			current_cmd->args[i] = ft_strdup(expanded);
            current_cmd->args[i+1] = NULL;
			free (expanded);
 		}
        else if (tmp->type == PIPE)
        {
            if (current_cmd)
                current_cmd->is_pipe = 1;
            current_cmd = NULL;
        }
        else if (tmp->type == TRUNC || tmp->type == INPUT || tmp->type == HEREDOC || tmp->type == APPEND)
        {
            redir_type = tmp->type; // <== Sauvegarde le type de redirection actuel
            if (tmp->next && tmp->next->type == ARG)
            {
                tmp = tmp->next;
                filename = ft_strdup(tmp->value);
                flag = (redir_type == APPEND || redir_type == HEREDOC) ? 1 : 0;
                fd = -1;
                // if (redir_type == INPUT || redir_type == HEREDOC)
                // {
                //     fd = open_file_cmd(filename);
                //     if (fd < 0)
                //     {
                //         perror(filename);
                //         free(filename);
                //         return NULL;
                //     }
                //     close(fd);
                //     if (current_cmd)
                //     {
                //         current_cmd->infile = filename;
                //         current_cmd->flag_in = flag;
                //     }
                //     else
                //     {
                //         pending_infile = filename;
                //         pending_flag_in = flag;
                //     }
                // }
            
                if (redir_type == HEREDOC)
                {
                    printf("detection HEREDOC\n");
                    printf("filename = %s\n", filename);
                    fd = handle_heredoc(filename, envcp); // Fonction pour gérer le heredoc
                    if (fd < 0)
                    {
                        perror(filename);
                        free(filename);
                        return NULL;
                    }
                    free(filename);
                    //close(fd);
                    if (current_cmd)
                    {
                        current_cmd->heredoc_fd = fd;
                        printf("DEBUG: heredoc_fd assigné : %d\n", current_cmd->heredoc_fd);
                        current_cmd->flag_in = 1; // Flag spécifique pour HEREDOC
                    }
                    else // heredoc avant une commande
                    {
                        pending_infile = filename;
                        pending_flag_in = 1;
                    }
                }
                else if (redir_type == INPUT)
                {
                    fd = open_file_cmd(filename);
                    if (fd < 0)
                    {
                        perror(filename);
                        free(filename);
                        return NULL;
                    }
                    close(fd);
                    if (current_cmd)
                    {
                        current_cmd->infile = filename;
                        current_cmd->flag_in = flag;
                    }
                    else
                    {
                        pending_infile = filename;
                        pending_flag_in = flag;
                    }
                }
                else if (redir_type == ERR_REDIR)
                {
                    fd = open_errfile(filename);
                    if (fd < 0)
                    {
                        perror(filename);
                        free(filename);
                        return NULL;
                    }
                    close(fd);
                    if (current_cmd)
                        current_cmd->errfile = filename;
                    else
                        pending_errfile = filename;
                }
                else // TRUNC or APPEND
                {
                    // int mode = O_WRONLY | O_CREAT | (flag ? O_APPEND : O_TRUNC);
                    fd = open_outfile(filename, flag);
                    if (fd < 0)
                    {
                        perror(filename);
                        free(filename);
                        return NULL;
                    }
                    close(fd);
                    if (current_cmd)
                    {
                        current_cmd->outfile = filename;
                        current_cmd->flag_out = flag;
                    }
                    else
                    {
                        pending_outfile = filename;
                        pending_flag_out = flag;
                    }
                }
            }
            else
            {
                fprintf(stderr, "minishell: syntax error near unexpected token '%s'\n", tmp->value);
                return NULL;
            }
        }
        tmp = tmp->next;
    }

    // Créer une commande vide si redirection seule
    if ((pending_outfile || pending_infile) && !current_cmd)
    {
        new_cmd = list_new(NULL); // Pas de commande
        new_cmd->heredoc_fd = -1;
        if (pending_outfile)
        {
            new_cmd->outfile = pending_outfile;
            new_cmd->flag_out = pending_flag_out;
            new_cmd->all_outfilles = pending_all_outfiles;
			new_cmd->heredoc_fd = -1;
            // add_outfile(&new_cmd->all_outfilles, pending_outfile, pending_flag_out);
        }
        if (pending_infile)
        {
            new_cmd->infile = pending_infile;
            new_cmd->flag_in = pending_flag_in;
        }
        if (pending_errfile)
            new_cmd->errfile = pending_errfile;

        if (!cmd_list)
            cmd_list = new_cmd;
        else
            add_bottom(&cmd_list, new_cmd);
    }
    // t_com_list  *iter = cmd_list;
    // while (iter)
    // {
    //     if (iter->args && iter->args[0])
    //     {
    //         free(iter->command);
    //         iter->command = ft_strdup(iter->args[0]);
    //     }
    //     iter = iter->next;
    // }
    return (cmd_list);
}

int	is_builting(char *cmd)
{
	return (
		ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}
