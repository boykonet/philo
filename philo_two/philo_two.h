#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# define TAKEN_FORK			"has taken a"
# define PUT_FORK			"put down a"
# define LEFT_FORK			"left fork"
# define RIGHT_FORK			"right fork"
# define PHILO_EAT			"is eating"
# define PHILO_SLEEP		"is sleeping"
# define PHILO_THINK		"is thinking"
# define PHILO_DIED			"died"
# define COUNT_PARAMS		"Count params is incorrect\n"
# define INCORRECT_PARAMS	"Input params is incorrect\n"
# define PTHREAD_JOIN		"Error: pthread_join\n"
# define PTHREAD_CREATE		"Error: pthread_create\n"

# define TRUE	1

# include <pthread.h>
/*
** pthread_create
** pthread_detach
** pthread_join
*/
#include <semaphore.h>
/*
** sem_open
** sem_close
** sem_post
** sem_wait
** sem_unlink
*/
# include <unistd.h>
/*
** write
** usleep
*/
# include <stdio.h>
/*
** printf
*/
# include <string.h>
/*
** memset
*/
# include <stdlib.h>
/*
** malloc
** free
*/
# include <sys/time.h>
/*
** gettimeofday
*/

typedef struct s_info
{
	struct timeval	start_time;
	struct timeval	real_time;
	sem_t			*forks;
	sem_t			*block_message;
	int				numb_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				numb_must_eat;
	int				must_eat;
	int				died;
}					t_info;

typedef struct s_philo
{
	struct timeval	start_time;
	struct timeval	real_time;
	t_info			*info;
	pthread_t		philo;
	int				num;
	int				ate;
}					t_philo;

int					ft_strtoi(const char *str, void **endptr);
int					ternar_int(int condition, int p1, int p2);
void				myusleep(int microsec);
void				print_message(t_philo *philo, long time,
						char *p1, char *p2);
long				lifetime(struct timeval *start_time,
						struct timeval *current_time, int ident);
void				init_philos(t_philo *philo, t_info *info);
int					init_info(t_info *info, int argc, char **argv);
void				destroy_init(t_info *info);
int					create_philo(t_philo *philo, int numb);
int					join_philo(t_philo *philo, int numb);
void				*routine(void *philo);

#endif