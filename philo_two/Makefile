NAME=philo_two

CFLAGS = -Wall -Wextra -Werror

CC = gcc

SRCS =			./philo_two.c \
				./philo_utils.c \
				./philo_utils2.c \
				./init.c \
				./create_join_destroy.c

OBJS = $(SRCS:%.c=%.o)

HEADERS =		./philo_two.h

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