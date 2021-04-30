#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# define TAKEN_FORK			"has taken a"
# define LEFT_FORK			"left fork"
# define RIGHT_FORK			"right fork"
# define PHILO_EAT			"is eating"
# define PHILO_SLEEP		"is sleeping"
# define PHILO_THINK		"is thinking"
# define PHILO_DIED			"\e[1;31mdied\e[0m"
/*
** 15
*/
# define COUNT_PARAMS		"\e[1;31mCount params is incorrect\e[0m\n"
/*
** 37
*/
# define INCORRECT_PARAMS	"\e[1;31mInput params is incorrect\e[0m\n"
/*
** 37
*/
# define PTH_JOIN			"\e[1;31mError: pthread_join\e[0m\n"
/*
** 31
*/
# define PTH_CREATE			"\e[1;31mError: pthread_create\e[0m\n"
/*
** 33
*/
# define PTH_M_INIT			"\e[1;31mError: pthread_mutex_init\e[0m\n"
/*
** 37
*/
# define PTH_M_DESTROY		"\e[1;31mError: pthread_mutex_destroy\e[0m\n"
/*
** 40
*/
# define PTH_M_LOCK			"\e[1;31mError: pthread_mutex_lock\e[0m\n"
/*
** 37
*/
# define PTH_M_UNLOCK		"\e[1;31mError: pthread_mutex_unlock\e[0m\n"
/*
** 37
*/
# define ERR_USLEEP			"\e[1;31mError: usleep\e[0m\n"
/*
** 25
*/
# define ERR_GETTIME		"\e[1;31mError: gettimeofday\e[0m\n"
/*
** 31
*/
# define ERR_FORK			"\e[1;31mError: fork\e[0m\n"
/*
** 23
*/

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
** fork
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
** exit
*/
# include <sys/time.h>
/*
** gettimeofday
*/
#include <signal.h>
/*
** kill
*/
#include <sys/wait.h>
/*
** waitpid
*/

typedef struct s_info
{
	struct timeval	start_time;
	struct timeval	real_time;
	sem_t			*forks;
	sem_t			*block_message;
	sem_t			*block_data;
	pid_t			pids[200];
	int				numb_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				numb_must_eat;
	int				must_eat;
	int				philos_eat;
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
int					myusleep(int microsec);
int					print_message(t_philo *philo, long time,
						char *p1, char *p2);
long				lifetime(struct timeval *start_time,
						struct timeval *current_time, int ident);
void				init_philos(t_philo *philo, t_info *info);
int					init_info(t_info *info, int argc, char **argv);
void				destroy_info(t_info *info);
int					create_process_treads(t_philo *philo, t_info *info);
int					join_philo(t_philo *philo, int numb);
void				*routine(void *philo);
void				check_die(t_philo *p, t_info *info);

#endif