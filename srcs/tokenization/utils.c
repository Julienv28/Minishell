#include "../includes/minishell.h"

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
