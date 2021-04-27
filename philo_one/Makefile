NAME = philo_one

CFLAGS = -Wall -Wextra -Werror

CC = gcc

SRCS =			./philo_one.c \
				./philo_utils.c \
				./init_structs.c \
				./create_join_destroy.c

OBJS = $(SRCS:%.c=%.o)

HEADERS =		./philo_one.h

INCLUDEDIR = ./

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(OBJS)

fcrean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
