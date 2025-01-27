SRCS = 	mandatory/utils.c mandatory/pipex.c  \
		libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c libft/ft_strlen.c \
		libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c libft/ft_strrchr.c libft/ft_strncmp.c \
		libft/ft_memcpy.c libft/ft_memmove.c libft/ft_memset.c libft/ft_memcmp.c libft/ft_memchr.c libft/ft_bzero.c \
		libft/ft_strnstr.c libft/ft_strlcpy.c libft/ft_strlcat.c libft/ft_atoi.c libft/ft_strdup.c libft/ft_calloc.c \
		libft/ft_substr.c libft/ft_strjoin.c libft/ft_split.c libft/ft_itoa.c libft/ft_strmapi.c \
		libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c \
		libft/ft_putnbr_fd.c libft/ft_strtrim.c  libft/ft_striteri.c
SRCS_BONUS = bonus/pipex_bonus.c bonus/utils_bonus.c \
		libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c libft/ft_strlen.c \
		libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c libft/ft_strrchr.c libft/ft_strncmp.c \
		libft/ft_memcpy.c libft/ft_memmove.c libft/ft_memset.c libft/ft_memcmp.c libft/ft_memchr.c libft/ft_bzero.c \
		libft/ft_strnstr.c libft/ft_strlcpy.c libft/ft_strlcat.c libft/ft_atoi.c libft/ft_strdup.c libft/ft_calloc.c \
		libft/ft_substr.c libft/ft_strjoin.c libft/ft_split.c libft/ft_itoa.c libft/ft_strmapi.c \
		libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c \
		libft/ft_putnbr_fd.c libft/ft_strtrim.c  libft/ft_striteri.c
NAME = pipex
NAME_BONUS = pipex_bonus
OBJS = ${SRCS:.c=.o}
OBJS_BONUS = ${SRCS_BONUS:.c=.o}
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
RM = rm -f

all: ${NAME}

bonus: ${NAME_BONUS}

.SECONDARY: ${OBJS} ${OBJS_BONUS}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

${NAME_BONUS}: ${OBJS_BONUS}
	${CC} ${CFLAGS} ${OBJS_BONUS} -o ${NAME_BONUS}

clean:
	${RM} ${OBJS} ${OBJS_BONUS}

fclean: clean
	${RM} ${NAME} ${NAME_BONUS}

re: fclean all