#include "../includes/minishell.h"

int	extract_word(char **str, int *i, char **word, int *start)
{
	while ((*str)[*i] && (*str)[*i] != ' ' && (*str)[*i] != '|' &&
			(*str)[*i] != '<' && (*str)[*i] != '>')
	{
		if ((*str)[*i] == '\'')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\'')
				(*i)++;
			(*i)++;
		}
		else if ((*str)[*i] == '\"')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\"')
				(*i)++;
			(*i)++;
		}
		else
			(*i)++;
	}
	*word = ft_strndup(*str + *start, *i - *start);
	if (!word)
		return (-1);
	return (0);
}

int	update_str_with_input(char **str, char *input)
{
	char	*tmp;
	char	*join;

	tmp = ft_strjoin(*str, input);
	if (!tmp)
		return (free(input), -1);
	if (check_mismatched_quotes(tmp) == 1)
	{
		join = ft_strjoin(tmp, "\n");
		free(tmp);
	}
	else
		join = tmp;
	free(*str);
	*str = join;
	free(input);
	return (0);
}

