#=== Project MName ===
NAME        = minishell

#=== Directories ===
SRC_DIR     = ./srcs
LIBFT_DIR   = ./libft
INC_DIR     = ./includes
OBJ_DIR		= ./obj

#=== External Libraries ===
LIBFT_FILE = libft.a
LIBFT = $(addprefix $(LIBFT_DIR)/, $(LIBFT_FILE))

#=== Compiler & Flags
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR) -I$(shell brew --prefix readline)/include
LDFLAGS     = -L$(shell brew --prefix readline)/lib -lreadline -L$(LIBFT_DIR) -lft

#=== Source Files ===
SRCS        = $(SRC_DIR)/main.c \
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
				$(SRC_DIR)/pipes/pipex_full.c \
				$(SRC_DIR)/pipes/pipex_simple.c \
				$(SRC_DIR)/pipes/pipex_utils.c \
				$(SRC_DIR)/commandes/echo.c \
				$(SRC_DIR)/commandes/cd_and_pwd.c \
				$(SRC_DIR)/commandes/exit.c \
				$(SRC_DIR)/commandes/utils.c \
				$(SRC_DIR)/commandes/export_and_env.c \
				$(SRC_DIR)/execution/exec.c \
				$(SRC_DIR)/signal.c \
				$(SRC_DIR)/commandes/unset.c

#=== Object Files ===
OBJ = ${SRCS:.c=.o}
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

#=== Compilation Rules ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

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
