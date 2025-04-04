#include "../includes/minishell.h"

void	init_cmds(t_com_list *command)
{
	command->command = NULL;
	command->outfile = 0;
	command->infile = 0;
	command->errfile = 0;
	command->flag_in = 0;
	command->flag_out = 0;
}
