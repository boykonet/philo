NAME = philo_one

CFLAGS = -g -Wall -Wextra -Werror -fsanitize=address #-fsanitize=leak

CC = gcc

SRCS =			./philo_one.c \
				./philo_utils.c \
				./init_structs.c \
				./create_join_destroy.c

OBJS = $(SRCS:%.c=%.o)

HEADERS =		./philo_one.h

INCLUDEDIR = ./

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
