/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:28:58 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 11:45:18 by juvitry          ###   ########.fr       */
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
# include <stdio.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>

# define MAX_ARGS 1024

// Définition des types de tokens
# define INPUT 1
# define HEREDOC 2
# define TRUNC 3
# define APPEND 4
# define PIPE 5
# define CMD 6
# define ARG 7
# define ERR_REDIR 8

extern int	g_sig_status;

// Structures
typedef struct s_file_list
{
	char				*filename;
	struct s_file_list	*next;
	int					flag;
}			t_file_list;

typedef struct s_com
{
	char				*cmd;
	char				**args;
	char				**envcp;
	char				*path;
	int					is_pipe;
	char				*outfile;
	char				*infile;
	char				*errfile;
	int					heredoc_fd;
	int					flag_in;
	int					flag_out;
	t_file_list			*all_outfilles;
	struct s_com		*next;
}						t_com;

typedef struct s_minishell
{
	char		**token;
	int			have_pipes;
}			t_minishell;

typedef struct s_tkn
{
	char			*value;
	int				type;
	int				is_quoted;
	struct s_tkn	*next;
}				t_tkn;

typedef struct s_expand
{
	char	**envcp;
	int		is_heredoc;
	int		quoted;
	int		expand_vars;
	int		*i;
}	t_expand;

typedef struct s_execinfo
{
	t_com	*curr;
	int		*prev_fd;
	int		pipefd[2];
	pid_t	pid;
	pid_t	*last_pid;
}	t_execinfo;

typedef struct s_filename
{
	char		*prefix;
	char		*pid_str;
	char		*count_str;
	char		*tmp;
	char		*final;
}	t_filename;

typedef struct s_parser_context
{
	t_com		*cmd_list;
	t_com		*current_cmd;
	t_com		*new_cmd;
	char		*filename;
	char		*pending_infile;
	int			pending_flag_in;
	char		*pending_outfile;
	int			pending_flag_out;
	char		*pending_errfile;
	t_file_list	*pending_all_outfiles;
	t_tkn		*current_token;
	char		**envcp;
}	t_parser_context;

typedef struct s_redirs
{
	int	in;
	int	out;
	int	err;
}	t_redirs;

typedef struct s_msh
{
	int		ex_status;
	char	**envcp;
	t_tkn	*tkn;
	t_com	*com;
}	t_msh;


// INITIALISATION
void				init_redirs(t_redirs *fds);
void				init_redirs(t_redirs *fds);
void				init_signals(void);

// Signaux
void				set_signal_action(void);
void				handler_sigint_prompt(int sig);
void				handler_sigint(int sig);
char				*append_char(char *res, char c);
void				heredoc_sigint_handler(int sig);
void				set_signals_child(void);
void				set_signals_parent(void);
void				reset_signals(void);

// Tokens
t_tkn				*add_token(t_tkn **head, char *s, int type, int is_quote);
t_tkn				*create_tokens(char **str, t_msh *msh);
char				*concat_command(char *current_command, char *new_part);
int					parse_redirection(char *str, int *i);
char				*add_symbol(int type);
int					handle_word(char **str, int *i, t_tkn **tkn, int *is_cmd);
void				restor_redir(int mem_fd_in, int mem_fd_out, int mem_fd_err);
int					ft_redir(t_com *cmd, int *fd_in, int *fd_out, int *fd_err);
int					open_file_cmd(char *infile);
int					open_outfile(char *outfile, int append);
int					open_errfile(char *errfile);
int					extract_word(char **str, int *i, char **word, int *start);
int					update_str_with_input(char **str, char *input);
int					process_pipe(char *str, int *i, t_tkn **tkn, int *is_cmd);
int					process_special_chars(char *str, int i);
int					process_redir(char *str, int *i, t_tkn **tkn, t_msh *msh);
int					process_word(char **str, int *i, t_tkn **tkn, int *is_cmd);
t_com				*fill_values(char **commands);
void				add_bottom(t_com **list, t_com *new);
t_com				*list_new(char *command);
int					handle_redir(char *s, int *i, t_tkn **tokens, t_msh *msh);
int					prompt_for_quotes(char **str);
int					is_directory(char *path);

// Tokens To Commands
t_com				*tokens_to_cmds(t_msh *msh);
void				handle_pipe_token(t_parser_context *ctx);
int					handle_arg_token(t_parser_context *ctx, t_msh *msh);
int					handle_redir_token(t_parser_context *ctx, t_msh *msh);
int					handle_cmd_token(t_parser_context *ctx, t_msh *msh);
t_com				*finalize_pending_redirs(t_parser_context *ctx);

// EXPAND VARIABLES
char				*replace_var_or_spe(char *str, char *res, t_expand *var);
char				*handle_special_cases(char *str, char *res, t_expand *var);
char				*replace_var(char *s, t_msh *msh, int is_hd, int expand);
char				*expand_env_variable(char *str, char *res, t_expand *var);
char				*handle_quote(char *str, char *res, t_expand *var);
char				*variable_name(char *str, t_expand *var, char *var_name);
char				*handle_brace_variable(char *str, char *res, t_expand *var);
char				*quote_dol(char *str, char *res, t_expand *var, int *quote);

// check
int					check_pipe(char *str, int i);
int					check_redirection(char *str, int *i);
int					check_mismatched_quotes(char *str);
int					check_input(char *str, int i);
int					check_isatty(void);

// Exit
int					ft_isnumber(char *str);

// Commandes
void				ft_echo(char **args, char ***envcp);
char				*add_space_if_needed(char *arg, char **envcp);
int					ft_cd(char **args, char ***envcp);
int					ft_pwd(char **args, char ***envcp);
int					ft_exit(char **args, int in_child, t_com *cmd);
void				cleanup_and_exit(int code, t_com *cmd);
int					handle_export(char **args, char ***envcp);
void				free_export_vars(char *key, char *value, char *replaced);
int					process_export(char *arg, char ***envcp, int *exit_status);
int					handle_export_error(char *replaced, char *arg);
char				*build_env_entry(char *key, char *value);
char				*get_env_value(char *name, char **envp);
char				*get_value_cleaned(char *name, char **envp);
void				ft_set_env(char *key, char *value, char ***envp);
void				ft_env(char **envp);
int					export_no_args(char **envp);
int					is_valid_name(char *name);
int					is_valid_n_flag(const char *str);
int					ft_unset(char **args, char ***envcp);
char				*unescape_backslashes(const char *str);

//Heredoc
char				*generate_tmp_filename(void);
void				heredoc_sigint_handler(int sig);
char				*handle_hd(char *limiter, char **envcp, int expand_var);
int					limiter_is_quoted(const char *str);
char				*hdclean(char *filename, char *limiter, int fd, int status);
int					print_heredoc_eof_error(void);
int					handle_heredoc_interrupt(char *line, int eof);
int					assign_hd_ctx(t_parser_context *ctx, char *heredoc_name);

// Exec
void				exec_cmd(char **args, char ***envcp);
int					is_builting(char *cmd);
int					exec_builting(char **args, char ***envcp, t_com *cmd);
char				*get_path(char *cmd, char **envp);
int					find_line(char **envp, char *path);
char				*search_path(char **paths, char *cmd);
int					exec_external(char **args, char ***envcp);
int					execute(t_com *cmds, char ***envcp);
void				execute_commands(t_com *cmd, char ***envcp);
int					handle_empty_command(t_com *cmd, t_redirs *fds);
int					handle_execution(t_com *cmd, char ***envcp, t_redirs *fds);
void				minishell_loop(t_msh *msh);
int					handle_null_tokens(t_msh *msh, char *input);
void				exit_shell(t_msh *msh);
int					handle_initial_errors(char **args);
void				fake_exit_builtin(char **args, t_com *cmds);
void				process_valid(char **args, char ***envcp, int *status);

//Pipes
void				wait_children(pid_t last_pid);
void				handle_pipes(char **args, t_com *cmds, char ***envcp);
void				child(t_com *curr, int prev, int pipefd[2], char ***envcp);
void				parent_pro(t_execinfo *ex);
int					exec_pipes(t_com *cmds, char **envcp);
int					exec(t_com *curr, int *prev, pid_t *lst_pid, char ***envcp);
int					setup_pipe(t_com *curr, int pipefd[2]);
char				**split_pipe_respect_quotes(const char *line);
int					parse_pipes(char **args);
int					has_pipe(t_com *command);

// FREE
void				free_tab(char **tab);
void				free_file_list(t_file_list *list);
char				*ft_strjoin_free(char *s1, char *s2);
char				*free_all(char *first, char *key, char *space, char *value);
void				ft_freeenvp(char **envcp);
void				free_cmd(t_com *command);
void				free_tokens(t_tkn *tokens);

// Utils
char				*remove_quotes_or_slash(char *str);
char				**ft_env_dup(char **envp);
char				**ft_realloc_env(char **envcp, char *new_entry);
void				init_cmds(t_com *command);
int					count_ags(char **args);
int					check_events(char *arg);
char				*clean_spaces(char *str);
void				add_outfile(t_file_list **list, char *filename, int flag);
int					syntax_error(t_msh *msh);
char				*expand_clean_word(char *word, char **envcp);
char				*export_s(char *arg, char **envp, char **key, char **value);
int					skip_spaces(char *str, int *i);
int					is_blank_line(const char *str);
int					is_valid_numeric_argument(char *str);
void				restore_and_close_fd(int *fd, int std_fd);

// Utils
unsigned long long	ft_atoull(const char *str);

#endif
