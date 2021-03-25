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

void 		*print(void *buff)
{
	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);
	for (int i = 0; i < 5; i++)
	{
		printf("[%d] hello, %s\n", i, (char*)buff);
		usleep(10000);
	}
	pthread_mutex_unlock(&mutex1);
	pthread_mutex_unlock(&mutex2);
	pthread_exit(0);
}

int			main(int argc, char **argv)
{
	pthread_t 		t1, t2;
	int 			num;
	int 			j;

	num = 0;
	j = 0;
	if ((argc < 5 || argc > 6) || check_argc(&argv[1]))
		printf(ERR_ARGC);
	else
	{
		pthread_mutex_init(&mutex, NULL);
		while (argv[1][j])
			num += argv[1][j++] - '0';
		pthread_create(&t1, NULL, print, "alo");
		pthread_create(&t2, NULL, print, "ku");
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	}
	return (0);
}
