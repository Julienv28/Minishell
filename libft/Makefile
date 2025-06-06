NAME =		libft.a

CC =		gcc
CFLAGS =	-Wall -Wextra -Werror

SRC =		ft_atoi.c ft_bzero.c ft_memset.c ft_memcpy.c \
			ft_memmove.c ft_memchr.c ft_memcmp.c ft_strlen.c \
			ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
			ft_isprint.c ft_toupper.c ft_tolower.c ft_strchr.c \
			ft_strrchr.c ft_strncmp.c ft_strcmp.c ft_strlcpy.c ft_strcpy.c ft_strlcat.c \
			ft_strnstr.c ft_atoi.c ft_calloc.c ft_strndup.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c get_next_line.c \
			ft_itoa.c ft_strmapi.c ft_putchar_fd.c ft_putstr_fd.c ft_strcat.c \
			ft_putendl_fd.c ft_putnbr_fd.c ft_bzero.c ft_striteri.c

OBJ =		${SRC:.c=.o}

.SUFFIXES:

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all:		${NAME}

${NAME}:	${OBJ}
			ar rc ${NAME} ${OBJ}
			ranlib ${NAME}

# .c.o:		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

clean:
			rm -f ${OBJ}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY :	all clean fclean re