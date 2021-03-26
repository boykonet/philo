#include "philo_one.h"

/*
** memset
** printf
** malloc
** free
** write
** usleep
** gettimeofday
** pthread_create
** pthread_detach
** pthread_join
** pthread_mutex_init
** pthread_mutex_destroy
** pthread_mutex_lock
** pthread_mutex_unlock
*/

typedef struct			s_philo
{
	int 				count_philo;
	int 				time_to_die;
	int 				time_to_eat;
	int 				time_to_sleep;
}						t_philo;

pthread_mutex_t			mutex1;
pthread_mutex_t 		mutex2;

int 		check_argc(char **argv)
{
	int 	i;
	int 	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
				return (1);
		}
		i++;
	}
	return (0);
}

void 		*eat(void *ptr)
{
	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);

	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
	pthread_exit(0);
}

int 		get_num(char *str)
{
	int 	i;
	int 	num;

	i = 0;
	num = 0;
	while (str[i])
		num += str[i++] - '0';
	return (num);
}

int			main(int argc, char **argv)
{
	t_philo			philo;
	pthread_t 		*t;
	int 			num;
	int 			j;
	int 			i;

	num = 0;
	j = 0;
	i = 0;
	if ((argc < 5 || argc > 6) || check_argc(&argv[1]))
		printf(ERR_ARGC);
	else
	{
		philo.count_philo = get_num(argv[1]);
		philo.time_to_die = get_num(argv[2]);
		philo.time_to_eat = get_num(argv[3]);
		philo.time_to_sleep = get_num(argv[4]);
		if (!(t = malloc(sizeof(pthread_t) * philo.count_philo)))
			return (0);
		while (i < philo.count_philo)
		{
			i++;
		}
		pthread_mutex_init(&mutex1, NULL);
		pthread_mutex_init(&mutex2, NULL);
		pthread_create(&t1, NULL, eat, "alo");
		pthread_create(&t2, NULL, eat, "ku");
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	}
	return (0);
}
