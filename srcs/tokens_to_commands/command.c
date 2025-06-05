/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:08:55 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 15:37:51 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	new_command = malloc(len_current + len_new + 2);
	if (new_command == NULL)
		return (NULL);
	if (current_command && *current_command)
	{
		ft_strcpy(new_command, current_command);
		ft_strcat(new_command, " ");
	}
	else
		new_command[0] = '\0';
	ft_strcat(new_command, new_part);
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

// t_com_list	*tokens_to_cmds(t_token *tokens, char **envcp)
// {
// 	t_com_list	*cmd_list = NULL;
// 	t_com_list	*current_cmd = NULL;
// 	t_token		*tmp = tokens;
// 	t_com_list	*new_cmd;
// 	char		*filename;
// 	char		*pending_outfile = NULL;
// 	int			pending_flag_out = -1;
// 	char		*pending_infile = NULL;
// 	int			pending_flag_in = -1;
// 	int			redir_type;
// 	char		*pending_errfile = NULL;
// 	t_file_list *pending_all_outfiles = NULL;

// 	while (tmp)
// 	{
// 		if (tmp->type == CMD)
// 		{
// 			if (!tmp->value)
// 			{
// 				fprintf(stderr, "Erreur : token CMD avec valeur NULL\n");
// 				tmp = tmp->next;
// 				continue;
// 			}
// 			char *expanded = replace_all_variables(tmp->value, envcp, 0);
// 			new_cmd = list_new(expanded);
// 			new_cmd->args = ft_calloc(MAX_ARGS, sizeof(char *));
// 			if (!new_cmd->args)
// 				return (NULL);
// 			new_cmd->args[0] = ft_strdup(expanded);
// 			new_cmd->args[1] = NULL;
// 			free(expanded);
// 			new_cmd->heredoc_fd = -1;
// 			if (pending_outfile)
// 			{
// 				new_cmd->outfile = pending_outfile;
// 				new_cmd->flag_out = pending_flag_out;
// 				new_cmd->all_outfilles = NULL;
// 				add_outfile(&new_cmd->all_outfilles, pending_outfile, 
//							pending_flag_out);
// 				pending_outfile = NULL;
// 				pending_flag_out = -1;
// 			}
// 			if (pending_infile)
// 			{
// 				new_cmd->infile = pending_infile;
// 				new_cmd->flag_in = pending_flag_in;
// 				pending_infile = NULL;
// 				pending_flag_in = -1;
// 			}
// 			if (!cmd_list)
// 				cmd_list = new_cmd;
// 			else
// 				add_bottom(&cmd_list, new_cmd);
// 			current_cmd = new_cmd;
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == ARG && current_cmd)
// 		{
// 			char *expanded = replace_all_variables(tmp->value, envcp, 0);
// 			int i = 0;
// 			while (current_cmd->args[i])
// 				i++;
// 			current_cmd->args[i] = ft_strdup(expanded);
// 			current_cmd->args[i + 1] = NULL;
// 			free(expanded);
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == PIPE)
// 		{
// 			if (current_cmd)
// 				current_cmd->is_pipe = 1;
// 			current_cmd = NULL;
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == TRUNC || tmp->type == INPUT
// 			|| tmp->type == HEREDOC || tmp->type == APPEND)
// 		{
// 			redir_type = tmp->type;
// 			tmp = tmp->next;
// 			if (!tmp || tmp->type != ARG)
// 				return (syntax_error(), NULL);
// 			filename = ft_strdup(tmp->value);
// 			if (!filename)
// 				return (NULL);
// 			if (redir_type == HEREDOC)
// 			{
// 				int expand = !limiter_is_quoted(filename);
// 				char *expanded = replace_all_variables(filename, envcp, 0);
// 				char *heredoc_name = handle_heredoc(expanded, envcp, expand);
// 				free(expanded);
// 				free(filename);
// 				if (!heredoc_name)
// 					return (NULL);
// 				if (!current_cmd)
// 				{
// 					pending_infile = ft_strdup(heredoc_name);
// 					if (!pending_infile)
// 						return (NULL);
// 				}
// 				else
// 				{
// 					free(current_cmd->infile);
// 					current_cmd->infile = ft_strdup(heredoc_name);
// 					if (!current_cmd->infile)
// 						return (NULL);
// 				}
// 				free(heredoc_name);
// 			}
// 			else if (redir_type == INPUT)
// 			{
// 				char *expanded = replace_all_variables(filename, envcp, 0);
// 				free(filename);
// 				if (current_cmd)
// 				{
// 					free(current_cmd->infile);
// 					current_cmd->infile = ft_strdup(expanded);
// 					if (!current_cmd->infile)
// 						return (NULL);
// 				}
// 				else
// 				{
// 					free(pending_infile);
// 					pending_infile = ft_strdup(expanded);
// 					if (!pending_infile)
// 						return (NULL);
// 				}
// 				free(expanded);
// 			}
// 			else // TRUNC ou APPEND
// 			{
// 				int flag = (redir_type == APPEND);
// 				int fd = open_outfile(filename, flag);
// 				if (fd < 0)
// 				{
// 					perror(filename);
// 					free(filename);
// 					return (NULL);
// 				}
// 				close(fd);
// 				if (current_cmd)
// 				{
// 					free(current_cmd->outfile);
// 					current_cmd->outfile = ft_strdup(filename);
// 					current_cmd->flag_out = flag;
// 				}
// 				else
// 				{
// 					free(pending_outfile);
// 					pending_outfile = ft_strdup(filename);
// 					pending_flag_out = flag;
// 				}
// 				free(filename);
// 			}
// 			tmp = tmp->next; // avance après l'argument
// 		}
// 		else
// 			tmp = tmp->next; // pour éviter blocage sur token inconnu
// 	}

// 	// Redirection seule en fin de ligne
// 	if ((pending_outfile || pending_infile) && !current_cmd)
// 	{
// 		new_cmd = list_new(NULL); // commande vide
// 		new_cmd->heredoc_fd = -1;
// 		if (pending_outfile)
// 		{
// 			new_cmd->outfile = pending_outfile;
// 			new_cmd->flag_out = pending_flag_out;
// 			new_cmd->all_outfilles = pending_all_outfiles;
// 		}
// 		if (pending_infile)
// 		{
// 			new_cmd->infile = pending_infile;
// 			new_cmd->flag_in = pending_flag_in;
// 		}
// 		if (pending_errfile)
// 			new_cmd->errfile = pending_errfile;

// 		if (!cmd_list)
// 			cmd_list = new_cmd;
// 		else
// 			add_bottom(&cmd_list, new_cmd);
// 	}
// 	return (cmd_list);
// }
