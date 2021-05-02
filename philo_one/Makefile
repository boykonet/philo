NAME = philo_one

CFLAGS = -g -Wall -Wextra -Werror

CC = gcc

SRCS =			./philo_one.c \
				./philo_utils.c \
				./philo_utils2.c \
				./create_join_destroy.c \
				./init.c

OBJS = $(SRCS:%.c=%.o)

HEADERS =		./philo_one.h

INCLUDEDIR = ./

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -pthread -I $(INCLUDEDIR) $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
