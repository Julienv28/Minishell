#include "../includes/minishell.h"

// ANALYSE DES COMMANDES

// Concaténer deux chaînes de caractères (une CMD et un nouvel ARG)
char *concat_command(char *current_command, char *new_part)
{
    char *new_command;
    int len_current;
    int len_new;

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

void free_file_list(t_file_list *list)
{
    t_file_list *tmp;

    while (list)
    {
        tmp = list;
        list = list->next;
        if (tmp->filename)
            free(tmp->filename);
        free(tmp);
    }
}

t_com_list *tokens_to_cmds(t_token *tokens)
{
    t_com_list      *cmd_list = NULL;
    t_com_list      *current_cmd = NULL;
    t_token         *tmp = tokens;
    t_com_list      *new_cmd;
    char            *filename;
    char            *pending_outfile = NULL;
    int             pending_flag_out = -1;
    char            *pending_infile = NULL;
    int             pending_flag_in = -1;
    int             flag;
    int             redir_type;
    t_file_list     *pending_all_outfiles = NULL;

    while (tmp)
    {
        // Affiche la valeur du token pour débogage
        if (tmp->type == CMD || (tmp->type == ARG && current_cmd == NULL))
        {
            if (!tmp->value)  // Vérifie si la valeur du token est NULL
            {
                fprintf(stderr, "Erreur : token CMD avec valeur NULL\n");
                tmp = tmp->next;
                continue;
            }
            new_cmd = list_new(tmp->value);
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
            current_cmd->command = concat_command(current_cmd->command, tmp->value);
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
                flag = (redir_type == APPEND || redir_type == HEREDOC) ? 1 : 0;
                filename = ft_strdup(tmp->value);
                if (!filename)
                    return NULL;
                if (redir_type == INPUT || redir_type == HEREDOC)
                {
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
                else // TRUNC or APPEND
                {
                    if (current_cmd)
                    {
                        current_cmd->outfile = filename;
                        current_cmd->flag_out = flag;
                        add_outfile(&current_cmd->all_outfilles, filename, flag);
                    }
                    else
                    {
                        pending_outfile = filename;
                        pending_flag_out = flag;
                        add_outfile(&pending_all_outfiles, filename, flag);
                    }
                }
            }
            else
            {
                fprintf(stderr, "minishell: syntax error near unexpected token '%s'\n", tmp->value);
                return NULL;  // Si pas de fichier après la redirection, on renvoie NULL pour signaler une erreur
            }
        }
        tmp = tmp->next;
    }
    return (cmd_list);
}

int is_builting(char *cmd)
{
    if (ft_strcmp(cmd, "exit") == 0)
        return (0);
    else if (ft_strcmp(cmd, "cd") == 0)
        return (0);
    else if (ft_strcmp(cmd, "pwd") == 0)
        return (0);
    else if (ft_strcmp(cmd, "echo") == 0)
        return (0);
    else if (ft_strcmp(cmd, "env") == 0)
        return (0);
    else if (ft_strcmp(cmd, "export") == 0)
        return (0);
    else if (ft_strcmp(cmd, "unset") == 0)
        return (0);
    return (1);
}
