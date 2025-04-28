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

void print_cmd_list(t_com_list *cmd_list)
{
    t_com_list *cmd = cmd_list;
    while (cmd)
    {
        printf("Command       : %s\n", cmd->command ? cmd->command : "(null)");
        if (cmd->infile)
            printf("Infile        : %s (flag_in: %d)\n", cmd->infile, cmd->flag_in);
        if (cmd->outfile)
            printf("Outfile       : %s (flag_out: %d)\n", cmd->outfile, cmd->flag_out);

        // Print all_outfiles if available
        if (cmd->all_outfilles)
        {
            printf("All Outfiles  :\n");
            t_file_list *f = cmd->all_outfilles;
            int j = 0;
            while (f)
            {
                printf("  [%d] %s\n", j++, f->filename);
                f = f->next;
            }
        }

        cmd = cmd->next;
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

void free_cmd(t_com_list *command)
{
    t_com_list *tmp;

    while (command)
    {
        tmp = command->next;

        if (command->command)
            free(command->command);
        if (command->infile)
            free(command->infile);
        if (command->outfile)
            free(command->outfile);
        if (command->errfile)
            free(command->errfile);
        if (command->all_outfilles)
            free(command->all_outfilles);

        free(command);
        command = tmp;
    }
}
