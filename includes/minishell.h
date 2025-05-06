/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:28:58 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/06 13:24:24 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Définition des codes de couleur ANSI
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"

// Declaration des packages
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h> // gerer les signaux

// Définition des types de tokens
#define INPUT 1
#define HEREDOC 2
#define TRUNC 3
#define APPEND 4
#define PIPE 5
#define CMD 6
#define ARG 7

// variable globale pour suivre l'état des erreurs
extern int	g_exit_status;

typedef struct s_file_list
{
	char				*filename;
	struct s_file_list	*next;
	int					flag; // ajouter
}	t_file_list;

typedef struct s_com_list
{
	char				*command;
	char				**args;
	char				**envcp;
	char				*path;
	int					is_pipe;
	char				*outfile;
	char				*infile;
	char				*errfile;
	int					flag_in; // 1 si redirection >>, 0 sinon
	int					flag_out;
	int					append;
	t_file_list			*all_outfilles;
	struct s_com_list	*next;
}	t_com_list;

typedef struct s_minishell
{
    char **token;
    int have_pipes;
} t_minishell;

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
} t_token;

// Message prompt + history (Oceane) ==> a ameliorer

// Signaux
void set_signal_action(void);
void signal_handler(int sig);
char *replace_all_variables(char *str);
void replace_exit_and_env_status(char **args);
char *replace_variable_or_special(char *str, int *i, char *res);
char *append_char(char *res, char c);

// Tokens
t_token *add_token(t_token **head, char *str, int type);
t_token *create_tokens(char **str);
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);
t_com_list *tokens_to_cmds(t_token *tokens);
char *concat_command(char *current_command, char *new_part);
int parse_redirection(char *str, int *i);
char *add_symbol(int type);
int handle_word(char **str, int *i, t_token **tokens, int *expect_cmd);
//int ft_redirection(t_com_list *command);
//void putback_direction(t_com_list *command, int mem_fd);
void restore_redirections(int mem_fd_in, int mem_fd_out, int mem_fd_err);
int ft_redirection(t_com_list *command, int *mem_fd_in, int *mem_fd_out, int *mem_fd_err);

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
int check_input(char *str, int i);

// Exit
int ft_isnumber(char *str);

// Commandes
void ft_echo(char *str, char **envcp);
void ft_cd(char **args, char ***envcp);
void ft_pwd(char **args);
void ft_exit(char **args);
void ft_export(char *arg, char ***envcp);
char *get_env_value(char *name, char **envp);
void ft_set_env(char *key, char *value, char ***envp);
void ft_env(char **envp);
int is_valid_name(char *name);
void ft_unset(char *key, char ***envcp);

// Exec
void	exec_cmd(t_com_list *command);
int		is_builting(char *cmd);
void	exec_builting(char **args, char ***envcp);
char	*get_path(char *cmd, char **envp);
int		find_line(char **envp, char *path);
char	*search_path(char **paths, char *cmd);
void	execute_cmd(t_com_list *cmd);

// Pipes (revoir les args pour pipex)
// void 		complex_pipex(t_com_list *command, int ac, char **args, char **envcp);
// void 		pipex_simple(t_com_list *command, char **args, char **envcp);
// int			simplified_gnl(char **line);
char		**split_pipe_respect_quotes(const char *line);
t_com_list	*parse_pipes(char *line);

// Utils
void exit_error(void);
int open_file(char *av, int i);
void ft_exec(char *av, char **envp);
void free_tab(char **tab);
void free_file_list(t_file_list *list);
char **split_args(const char *s, char sep);
char *remove_quotes_or_slash(char *str);
void free_cmd(t_com_list *command);
char **ft_env_dup(char **envp);
void ft_freeenvp(char **envcp);
char *ft_srjoin3(char *s1, char *s2, char *s3);
char **ft_realloc_env(char **envcp, char *new_entry);
void init_cmds(t_com_list *command);
int parse_args_echo(char **args);
int count_ags(char **args);
int check_events(char *arg);

void print_cmd_list(t_com_list *cmd_list);
void add_outfile(t_file_list **list, char *filename, int flag);

#endif