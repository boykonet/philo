#include "philo_three.h"

int			ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}

int	ft_strtoi(const char *str, void **endptr)
{
	int	flag;
	long int	digit;

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
		*endptr = (void *)str;
	return ((int)(digit * flag));
}

int	ternar_int(int condition, int p1, int p2)
{
	if (condition)
		return (p1);
	return (p2);
}

int	myusleep(int microsec)
{
	struct timeval	start_t;
	struct timeval	real_t;
	long int		num;

	if (gettimeofday(&start_t, NULL) == -1)
		return (ternar_int(write(2, ERR_GETTIME, 20) > 0, -1, 0));
	while (TRUE)
	{
		if (gettimeofday(&real_t, NULL) == -1)
			return (ternar_int(write(2, ERR_GETTIME, 20) > 0, -1, 0));
		num = (real_t.tv_sec - start_t.tv_sec) * 1000000
			+ (real_t.tv_usec - start_t.tv_usec);
		if (num >= microsec)
			break ;
		if (usleep(50) == -1)
			return (ternar_int(write(2, ERR_USLEEP, 14) > 0, -1, 0));
	}
	return (0);
}

int	print_message(t_philo *philo, long time, char *p1, char *p2)
{
	if (philo->info->died == 1 || philo->info->philos_eat == 1)
		return (0);
	sem_wait(philo->info->block_message);
	if (!philo->info->died && !philo->info->philos_eat)
	{
		printf("%6ld %d %s", time, philo->num + 1, p1);
		if (p2)
			printf(" %s", p2);
		printf("\n");
		if (!ft_strcmp(PHILO_DIED, p1))
			philo->info->died = 1;
	}
	sem_post(philo->info->block_message);
	return (0);
}

long	lifetime(struct timeval *start_time,
		struct timeval *current_time, int ident)
{
	if (gettimeofday(current_time, NULL) == -1)
		return (-1);
	if (ident)
	{
		start_time->tv_sec = current_time->tv_sec;
		start_time->tv_usec = current_time->tv_usec;
	}
	return (((current_time->tv_sec * 1000 + current_time->tv_usec / 1000)
	- (start_time->tv_sec * 1000 + start_time->tv_usec / 1000)));
}
