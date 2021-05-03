#include "philo_three.h"

int	myusleep(int microsec)
{
	struct timeval	start_t;
	struct timeval	real_t;
	long int		num;

	if (gettimeofday(&start_t, NULL) == -1)
		return (ternar_int(write(2, ERR_GETTIME, 31) > 0, -1, 0));
	while (TRUE)
	{
		if (gettimeofday(&real_t, NULL) == -1)
			return (ternar_int(write(2, ERR_GETTIME, 31) > 0, -1, 0));
		num = (real_t.tv_sec - start_t.tv_sec) * 1000000
			+ (real_t.tv_usec - start_t.tv_usec);
		if (num >= microsec)
			break ;
		if (usleep(100) == -1)
			return (ternar_int(write(2, ERR_USLEEP, 25) > 0, -1, 0));
	}
	return (0);
}

int	message(t_philo *ph, long time, char *p1, char *p2)
{
	sem_wait(ph->info->block_message);
	if (!ph->info->died && !ph->info->philos_eat)
	{
		printf("%6ld %d %s", time, ph->num + 1, p1);
		if (p2)
			printf(" %s", p2);
		printf("\n");
		if (!ft_strcmp(PH_DIED, p1))
			return (1);
	}
	sem_post(ph->info->block_message);
	return (0);
}

long	lifetime(sem_t *block_time, struct timeval *start_time,
		struct timeval *curr_time, int ident)
{
	sem_wait(block_time);
	if (gettimeofday(curr_time, NULL) == -1)
		return (-1);
	if (ident)
	{
		start_time->tv_sec = curr_time->tv_sec;
		start_time->tv_usec = curr_time->tv_usec;
	}
	sem_post(block_time);
	return (((curr_time->tv_sec * 1000 + curr_time->tv_usec / 1000)
			- (start_time->tv_sec * 1000 + start_time->tv_usec / 1000)));
}
