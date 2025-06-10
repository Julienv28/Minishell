#include "../includes/minishell.h"

int	open_file_cmd(char *infile)
{
	int	fd;

	if (!infile || !*infile)
		return (-1);
	fd = open(infile, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	open_outfile(char *outfile, int append)
{
	int	fd;

	if (!outfile || !*outfile)
		return (-1);
	if (append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	open_errfile(char *errfile)
{
	int	fd;

	if (!errfile || !*errfile)
		return (-1);
	fd = open(errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	return (fd);
}
