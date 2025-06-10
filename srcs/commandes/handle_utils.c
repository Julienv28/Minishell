#include "../../includes/minishell.h"

char	*expand_arg(char *arg, char **envp)
{
	char	*varname;
	char	*value;

	if (!arg)
		return (NULL);
	if (arg[0] != '$')
		return (ft_strdup(arg));
	varname = ft_substr(arg, 1, ft_strlen(arg) - 1);
	if (!varname)
		return (NULL);
	value = get_env_value(varname, envp);
	free(varname);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

int	handle_initial_errors(char **args)
{
	int		i;
	int		status;
	char	*clean_arg;

	i = 1;
	status = 0;
	while (args[i])
	{
		clean_arg = remove_quotes_or_slash(args[i]);
		free(clean_arg);
		i++;
	}
	return (status);
}
/*
int	expand_and_check(char **args, char ***envcp)
{
	int		i;
	int		all_empty;
	char	*expanded;

	i = 1;
	all_empty = 1;
	while (args[i])
	{
		expanded = expand_arg(args[i], *envcp);
		printf("expand = %s\n", expanded);
		free(args[i]);
		args[i] = expanded;
		if (expanded && expanded[0] != '\0')
			all_empty = 0;
		i++;
	}
	return (all_empty);
}*/
/*void	process_valid_exports(char **args, char ***envcp, int *status)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')
			process_export_entry(args[i], envcp, status);
		i++;
	}
}*/
void	process_valid_exports(char **args, char ***envcp, int *status)
{
	int		i;
	char	*expanded;

	i = 1;
	while (args[i])
	{
		// Expansion de l'argument brut
		expanded = replace_all_variables(args[i], *envcp, 0, 1);
		if (!expanded || expanded[0] == '\0')
		{
			// Si l'expansion donne une chaîne vide : export sans argument
			free(expanded);
			if (*status == 0)
				*status = export_no_args(*envcp); // à toi d’implémenter
			i++;
			continue;
		}
		process_export_entry(expanded, envcp, status);
		free(expanded);
		i++;
	}
}