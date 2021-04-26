#include "philo_one.h"

int			ft_strtoi(const char *str, void **endptr)
{
	int		flag;
	long	digit;

	flag = 1;
	digit = 0;
	if (str == NULL)
		return (0);
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t'
	|| *str == '\r' || *str == '\f' || *str == '\v'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			flag = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		digit = digit * 10 + *str++ - '0';
	if (endptr)
		*endptr = (void*)str;
	return ((int)(digit * flag));
}

int			ternar_int(int condition, int p1, int p2)
{
	if (condition)
		return p1;
	else
		return p2;
}

void		myusleep(int microsec)
{
	struct timeval		start_time;
	struct timeval		real_time;
	long int 			num;

	gettimeofday(&start_time, NULL);
	while (TRUE)
	{
		gettimeofday(&real_time, NULL);
		num = (real_time.tv_sec - start_time.tv_sec) * 1000000 + (real_time.tv_usec - start_time.tv_usec);
		if (num >= microsec)
			break ;
		usleep(100);
	}
}

void 					print_message(t_philo *philo, long time, char *p1, char *p2)
{
	pthread_mutex_lock(&philo->info->block);
	if (!strcmp(p1, PHILO_DIED))
		printf("\e[1;31m%ld %d %s\e[0m", time, philo->num + 1, p1);
	else
		printf("%ld %d %s", time, philo->num + 1, p1);
	if (p2)
		printf(" %s", p2);
	printf("\n");
	pthread_mutex_unlock(&philo->info->block);
}

long 					lifetime(struct timeval *start_time, struct timeval *current_time, int ident)
{
	if (gettimeofday(current_time, NULL) == -1)
		return (0);
	if (ident)
	{
		start_time->tv_sec = current_time->tv_sec;
		start_time->tv_usec = current_time->tv_usec;
	}
	return ((current_time->tv_sec - start_time->tv_sec) * 1000
			+ (current_time->tv_usec - start_time->tv_usec) / 1000);
}