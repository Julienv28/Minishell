NAME        = minishell

SRC_DIR     = ./srcs
LIBFT_DIR   = ./libft
INC_DIR     = ./includes

LIBFT       = $(LIBFT_DIR)/libft.a
SRCS        = $(SRC_DIR)/main.c $(SRC_DIR)/parse_args.c $(SRC_DIR)/initialisation_args.c $(SRC_DIR)/split_args.c \
              $(SRC_DIR)/error_handler.c $(SRC_DIR)/tokenization/tokens.c $(SRC_DIR)/tokenization/utils.c \
			  $(SRC_DIR)/tokenization/command.c $(SRC_DIR)/tokenization/redirection.c $(SRC_DIR)/tokenization/open_file.c\
			  $(SRC_DIR)/tokenization/check_cmd.c $(SRC_DIR)/tokenization/arguments.c \
			  $(SRC_DIR)/pipes/pipex_full.c $(SRC_DIR)/pipes/pipex_simple.c $(SRC_DIR)/pipes/pipex_utils.c \
			  $(SRC_DIR)/commandes/echo.c $(SRC_DIR)/commandes/cd_and_pwd.c $(SRC_DIR)/commandes/exit.c $(SRC_DIR)/commandes/utils.c \
			  $(SRC_DIR)/commandes/export_and_env.c $(SRC_DIR)/execution/exec.c
OBJS        = ${SRCS:.c=.o}

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_DIR) -lft -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all re clean fclean
