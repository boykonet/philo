#include "philo_one.h"

int			ft_atoi(const char *str)
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
	while (*str)
		digit = digit * 10 + *str++ - '0';
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