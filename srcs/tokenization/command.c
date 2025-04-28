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

t_com_list *tokens_to_cmds(t_token *tokens)
{
    t_com_list *cmd_list;
    t_com_list *current_cmd;
    t_com_list *new_cmd;
    t_token *tmp;

    cmd_list = NULL;
    current_cmd = NULL;
    tmp = tokens;
    while (tmp)
    {
        // Si le token est de type CMD, créer une nouvelle commande
        if (tmp->type == CMD)
        {
            new_cmd = list_new(tmp->value);
            if (!cmd_list)
                cmd_list = new_cmd; // Si la liste est vide, l'ajoute comme premier élément
            else
                add_bottom(&cmd_list, new_cmd); // Sinon, l'ajoute à la fin de la liste
            current_cmd = new_cmd;
            printf("Détection d'une cmd: %s\n", tmp->value);
        }
        else if (tmp->type == ARG && current_cmd)
        {
            printf("Détection d'un arg: %s\n", tmp->value);
            // Si c'est un argument, on l'ajoute à la commande en cours
            current_cmd->command = concat_command(current_cmd->command, tmp->value);
        }
        else if (tmp->type == PIPE && current_cmd)
        {
            // Si c'est un pipe, on marque la commande comme étant un pipe
            current_cmd->is_pipe = 1;
        }
        else if ((tmp->type == TRUNC || tmp->type == INPUT || tmp->type == HEREDOC || tmp->type == APPEND) && current_cmd)
        {
            if (tmp->next != NULL && tmp->next->type == ARG)
            {
                if (tmp->type == INPUT)
                {
                    tmp = tmp->next;
                    current_cmd->infile = tmp->value;
                    current_cmd->flag_in = 0;
                }
                else if (tmp->type == HEREDOC)
                {
                    tmp = tmp->next;
                    current_cmd->infile = tmp->value;
                    current_cmd->flag_in = 1;
                }
                else if (tmp->type == TRUNC)
                {
                    tmp = tmp->next;
                    current_cmd->outfile = tmp->value;
                    current_cmd->flag_out = 0;
                    // Ajouter à la liste de fichiers de sortie
                    add_outfile(&current_cmd->all_outfilles, tmp->value);
                }
                else if (tmp->type == APPEND)
                {
                    tmp = tmp->next;
                    current_cmd->outfile = tmp->value;
                    current_cmd->flag_out = 1;
                    add_outfile(&current_cmd->all_outfilles, tmp->value);
                }
            }
        }
        if (!tmp->next)
            break;
        tmp = tmp->next;
    }
    return cmd_list;
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
