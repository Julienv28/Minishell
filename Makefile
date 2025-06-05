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

READLINE_INCLUDE = /opt/homebrew/opt/readline/include 
READLINE_LIB = /opt/homebrew/opt/readline/lib

CFLAGS      = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR) -I$(READLINE_INCLUDE)
LDFLAGS     = -L$(READLINE_LIB) -lreadline -L$(LIBFT_DIR) -lft

#=== Source Files ===
SRCS        = 	$(SRC_DIR)/main.c \
				$(SRC_DIR)/pipes_manager.c \
				$(SRC_DIR)/initialisation_args.c \
				$(SRC_DIR)/split_args.c \
				$(SRC_DIR)/error_handler.c \
				$(SRC_DIR)/tokenization/tokens.c \
				$(SRC_DIR)/tokenization/utils.c \
				$(SRC_DIR)/tokenization/command.c \
				$(SRC_DIR)/tokenization/redirection.c \
				$(SRC_DIR)/tokenization/open_file.c \
				$(SRC_DIR)/tokenization/check_cmd.c \
				$(SRC_DIR)/tokenization/arguments.c \
				$(SRC_DIR)/dollard.c \
				$(SRC_DIR)/commandes/echo.c \
				$(SRC_DIR)/commandes/cd.c \
				$(SRC_DIR)/commandes/pwd.c \
				$(SRC_DIR)/commandes/exit.c \
				$(SRC_DIR)/commandes/utils.c \
				$(SRC_DIR)/commandes/export.c \
				$(SRC_DIR)/commandes/env.c \
				$(SRC_DIR)/commandes/utils_export_unset_env.c \
				$(SRC_DIR)/execution/exec.c \
				$(SRC_DIR)/signal.c \
				$(SRC_DIR)/commandes/unset.c \
				$(SRC_DIR)/execution/execute_commands.c \
				$(SRC_DIR)/execution/exec_external.c \
				$(SRC_DIR)/execution/exec_pipes.c \
				$(SRC_DIR)/execution/utils_pipes.c \
				$(SRC_DIR)/execution/exec_builtings.c \
				$(SRC_DIR)/execution/exec_utils.c \
				$(SRC_DIR)/split_pipes.c

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
	@make clean -sC $(LIBFT_DIR)
	@echo "\033[1;32m==> Clean complete.\033[0m"

fclean: clean
	@echo "\033[0;31m==> Removing executable $(NAME)...\033[0m"
	@rm -f $(NAME)
	@make fclean -sC $(LIBFT_DIR)
	@echo "\033[1;32m==> Full clean complete.\033[0m"

re: fclean all

.PHONY: all re clean fclean