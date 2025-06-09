#=== Project Name ===
NAME        = minishell

#=== Directories ===
SRC_DIR     = ./srcs
LIBFT_DIR   = ./libft
INC_DIR     = ./includes
OBJ_DIR		= ./obj

#=== External Libraries ===
LIBFT_FILE = libft.a
LIBFT = $(addprefix $(LIBFT_DIR)/, $(LIBFT_FILE))

#=== Compiler & Flags ===
CC          = gcc

READLINE_INCLUDE = /usr/include
READLINE_LIB = /usr/lib

READLINE_INCLUDE = /usr/local/opt/readline/include
READLINE_LIB = /usr/local/opt/readline/lib

#====== MacOS Flags======
READLINE_INCLUDE = /opt/homebrew/opt/readline/include 
READLINE_LIB = /opt/homebrew/opt/readline/lib

CFLAGS      = -Wall -Wextra -Werror -g -I $(INC_DIR) -I $(LIBFT_DIR) -I$(READLINE_INCLUDE)
LDFLAGS     = -L$(READLINE_LIB) -lreadline -L$(LIBFT_DIR) -lft

.SUFFIXES:

#=== Source Files ===
SRCS        = 	$(SRC_DIR)/main.c \
				$(SRC_DIR)/minishell_loop.c \
				$(SRC_DIR)/pipes_manager.c \
				$(SRC_DIR)/split_args.c \
				$(SRC_DIR)/error_handler.c \
				$(SRC_DIR)/tokenization/tokens.c \
				$(SRC_DIR)/tokenization/utils.c \
				$(SRC_DIR)/tokenization/utils2.c \
				$(SRC_DIR)/tokenization/processing_tokens.c \
				$(SRC_DIR)/tokenization/ft_redirection.c \
				$(SRC_DIR)/tokenization/handle_redirection.c \
				$(SRC_DIR)/tokenization/open_file.c \
				$(SRC_DIR)/tokenization/check_cmd.c \
				$(SRC_DIR)/tokenization/arguments.c \
				$(SRC_DIR)/tokenization/arguments_2.c \
				$(SRC_DIR)/expand_var/dollard_2.c \
				$(SRC_DIR)/expand_var/dollard.c \
				$(SRC_DIR)/expand_var/replacement.c \
				$(SRC_DIR)/expand_var/handle_quote_dollard.c \
				$(SRC_DIR)/commandes/echo.c \
				$(SRC_DIR)/commandes/echo_2.c \
				$(SRC_DIR)/commandes/cd.c \
				$(SRC_DIR)/commandes/pwd.c \
				$(SRC_DIR)/commandes/exit.c \
				$(SRC_DIR)/commandes/building_utils.c \
				$(SRC_DIR)/commandes/export.c \
				$(SRC_DIR)/commandes/export_2.c \
				$(SRC_DIR)/commandes/env.c \
				$(SRC_DIR)/commandes/free_building.c \
				$(SRC_DIR)/commandes/utils.c \
				$(SRC_DIR)/execution/exec.c \
				$(SRC_DIR)/signal.c \
				$(SRC_DIR)/commandes/unset.c \
				$(SRC_DIR)/execution/execute_commands.c \
				$(SRC_DIR)/execution/exec_external.c \
				$(SRC_DIR)/execution/exec_pipes.c \
				$(SRC_DIR)/execution/utils_pipes.c \
				$(SRC_DIR)/execution/exec_builtings.c \
				$(SRC_DIR)/execution/exec_utils.c \
				$(SRC_DIR)/tokens_to_commands/command.c\
				$(SRC_DIR)/tokens_to_commands/ft_heredocs.c \
				$(SRC_DIR)/tokens_to_commands/heredoc_utils.c \
				$(SRC_DIR)/tokens_to_commands/tkn_to_cmds.c \
				$(SRC_DIR)/tokens_to_commands/handle_arg.c \
				$(SRC_DIR)/tokens_to_commands/handle_cmd.c \
				$(SRC_DIR)/tokens_to_commands/handle_pipes.c \
				$(SRC_DIR)/tokens_to_commands/handle_redirections.c \
				$(SRC_DIR)/tokens_to_commands/finalize_redirs.c \
				$(SRC_DIR)/tokens_to_commands/create_cmd.c \
				$(SRC_DIR)/split_pipes.c \
				$(SRC_DIR)/initialisation.c

#=== Object Files ===
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#=== Compilation Rules ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#=== Build Targets ===

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "\033[0;33m\n==> Compiling libft...\033[0m"
	@make -C $(LIBFT_DIR)
	@echo "\033[1;32m==> libft compiled successfully.\033[0m"

$(NAME): $(LIBFT) $(OBJS)
	@echo "\033[0;33m\n==> Linking $(NAME)...\033[0m"
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "\033[1;32m==> $(NAME) built successfully.\033[0m"

#=== Cleaning rules===
clean:
	@echo "\033[0;31m==> Cleaning object files...\033[0m"
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean 
	@echo "\033[1;32m==> Clean complete.\033[0m"

fclean: clean
	@echo "\033[0;31m==> Removing executable $(NAME)...\033[0m"
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean
	@echo "\033[1;32m==> Full clean complete.\033[0m"

re: fclean all

.PHONY: all re clean fclean