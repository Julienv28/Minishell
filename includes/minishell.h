/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:28:58 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/04 15:00:49 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Définition des codes de couleur ANSI
# define RESET "\x1b[0m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"

// Declaration des packages
# include <unistd.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define TIMEOUT_ITERATIONS 10000

// Définition des types de tokens
# define INPUT 1
# define HEREDOC 2
# define TRUNC 3
# define APPEND 4
# define PIPE 5
# define CMD 6
# define ARG 7

typedef struct s_com_list
{
	char				*command;
	int					is_pipe;
    char				*outfile;
    char				*infile;
    char				*errfile;
    int					flag_in; // 1 si redirection >>, 0 sinon
    int					flag_out;
	struct s_com_list	*next;
}	t_com_list;

typedef struct s_minishell
{
	char	**token;
	int		have_pipes;
}	t_minishell;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

// Message prompt + history (Oceane) ==> a ameliorer

// Tokens
t_token *add_token(t_token **head, char *str, int type);
t_token *create_tokens(char **str);
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);
t_com_list *tokens_to_cmds(t_token *tokens);
char *concat_command(char *current_command, char *new_part);
int parse_redirection(char *str, int *i);
char *add_symbol(int type);
char *handle_word(char **str, int *i, t_token **tokens, int *expect_cmd);
void ft_redirection(t_com_list *command);
int open_file_cmd(char *infile);
int open_outfile(char *outfile, int append);
int open_errfile(char *errfile);

// Parsing
t_com_list *fill_values(char **commands);
void add_bottom(t_com_list **list, t_com_list *new);
t_com_list *list_new(char *command);
int handle_redirection(char *str, int *i, t_token **tokens);
int prompt_for_quotes(char **str);

// check
int check_pipe(char *str, int i);
int check_redirection(char *str, int *i);
int check_mismatched_quotes(char *str);

// Exec
void exec_cmd(t_com_list *command);

// Pipes (revoir les args pour pipex)
void complex_pipex(int ac, char **av, char **envp);
void pipex_simple(char **av, char **envp);
int simplified_gnl(char **line);

// Utils
void exit_error(void);
int open_file(char *av, int i);
void ft_exec(char *av, char **envp);

#endif