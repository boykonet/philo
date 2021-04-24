#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# define TAKEN_FORK			"has taken a"
# define PUT_FORK			"put down a"
# define LEFT_FORK			"left fork"
# define RIGHT_FORK			"right fork"
# define PHILO_EAT			"is eating"
# define PHILO_SLEEP			"is sleeping"
# define PHILO_THINK			"is thinking"
# define PHILO_DIED			"died"
# define COUNT_PARAMS		"Count params is incorrect\n"
# define CORRECT_PARAMS		"Input params is incorrect\n"

# define FALSE				0
# define TRUE				1

# include <pthread.h>		/*
							** pthread_create
							** pthread_detach
							** pthread_join
							** pthread_mutex_init
							** pthread_mutex_destroy
							** pthread_mutex_lock
							** pthread_mutex_unlock
							*/
# include <unistd.h>			/*
							** write
							** usleep
							*/
# include <stdio.h>			/*
							** printf
							*/
# include <string.h>			/*
							** memset
							*/
# include <stdlib.h>			/*
							** malloc
							** free

							*/
# include <sys/time.h>		/*
							** gettimeofday
							*/

typedef struct			s_info
{
	pthread_mutex_t		*forks;
	pthread_mutex_t 	block;
	int					numb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					numb_must_eat;
	int					must_eat;
	int 				died;
	struct timeval		start_time;
	struct timeval		real_time;
}						t_info;

typedef struct			s_philo
{
	t_info				*info;
	pthread_t			philo;
	long int			life_cycle;
	int 				left_fork;
	int 				right_fork;
	int					num;
	int 				ate;
	struct timeval		start_time;
	struct timeval		real_time;
}						t_philo;

int						ft_atoi(const char *str);
int						ternar_int(int condition, int p1, int p2);
void					myusleep(int microsec);
void 					print_message(t_philo *philo, long time, char *p1, char *p2);
long 					lifetime(struct timeval *start_time, struct timeval *current_time, int ident);

#endif