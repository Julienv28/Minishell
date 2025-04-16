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
        //printf("Traitement du token : %s (type %d)\n", tmp->value, tmp->type);
        if (tmp->type == CMD) // Si CMD, on crée une nouvelle commande.
        {
            new_cmd = list_new(tmp->value);
            if (!cmd_list)
                cmd_list = new_cmd;
            else
                add_bottom(&cmd_list, new_cmd);
            current_cmd = new_cmd;
        }
        else if (tmp->type == ARG && current_cmd) // Si ARG, on l'ajoute à la commande en cours.
            current_cmd->command = concat_command(current_cmd->command, tmp->value);
        else if (tmp->type == PIPE && current_cmd) // Si PIPE, on marque la fin de la commande actuelle.
            current_cmd->is_pipe = 1;
        else if ((tmp->type == TRUNC || tmp->type == INPUT || tmp->type == HEREDOC || tmp->type == APPEND) && (current_cmd))
        {
            if (tmp->next != NULL && tmp->next->type == ARG)
            {
                //printf("type = %d\n", tmp->type);
                //printf("valeur type = %s\n", tmp->value);
                if (tmp->type == INPUT)
                {
                    tmp = tmp->next;
                    current_cmd->infile = tmp->value;
                    //current_cmd->infile = ft_strdup(tmp->value);
                    current_cmd->flag_in = 0;
                    // printf("Redirection d'entrée détectée : %s\n", current_cmd->infile);
                }
                else if (tmp->type == HEREDOC)
                {
                    tmp = tmp->next;
                    current_cmd->infile = tmp->value;
                    //current_cmd->infile = ft_strdup(tmp->value);
                    current_cmd->flag_in = 1;
                    // printf("Redirection d'entrée détectée : %s\n", current_cmd->infile);
                }
                else if (tmp->type == TRUNC)
                {
                    tmp = tmp->next;
                    current_cmd->outfile = tmp->value;
                    //current_cmd->outfile = ft_strdup(tmp->value);
                    current_cmd->flag_out = 0;
                    // printf("Redirection de sortie détectée: fichier = %s\n", current_cmd->outfile);
                }
                else if (tmp->type == APPEND)
                {
                    tmp = tmp->next;
                    current_cmd->outfile = tmp->value;
                    //current_cmd->outfile = ft_strdup(tmp->value);
                    current_cmd->flag_out = 1;
                    // printf("Redirection de sortie (>>) détectée: %s\n", current_cmd->outfile);
                }
            }
        }
        if (!tmp->next) // Si tmp->next est NULL, sortir de la boucle.
            break;
        tmp = tmp->next; // Avance tmp une deuxième fois après avoir traité l'argument
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
    else if (ft_strcmp(cmd, "export") == 0)
        return (0);
    // else if (ft_strcmp(cmd, "unset") == 0)
    //     return (0);
    return (1);
}