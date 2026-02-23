NAME		= pipex

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Iinclude

SRCS		= src/main.c \
			  src/exec.c \
			  src/utils.c \
			  src/split_path.c \
			  src/split_command.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT_OBJS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
