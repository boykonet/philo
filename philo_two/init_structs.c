#include "philo_two.h"

static int	params_for_philo(char *str, int *data)
{
	void	*endptr;

	*data = ft_strtoi(str, &endptr);
	if (*(char *)endptr)
		return (0);
	return (1);
}

int	init_info(t_info *info, int argc, char **argv)
{
	info->must_eat = ternar_int((argc == 6), 0, -1);
	if (!params_for_philo(argv[1], &info->numb_of_philo))
		return (0);
	if (!params_for_philo(argv[2], &info->time_to_die))
		return (0);
	if (!params_for_philo(argv[3], &info->time_to_eat))
		return (0);
	if (!params_for_philo(argv[4], &info->time_to_sleep))
		return (0);
	info->numb_must_eat = 0;
	if (!info->must_eat)
		if (!params_for_philo(argv[5], &info->numb_must_eat))
			return (0);
	if (info->numb_of_philo <= 2 || info->time_to_die <= 0
		|| info->time_to_eat <= 0 || info->time_to_sleep <= 0
		|| (!info->must_eat && info->numb_must_eat <= 0))
	{
		write(2, INCORRECT_PARAMS, 26);
		return (0);
	}
	info->died = 0;
	sem_unlink("forks");
	sem_unlink("block_message");
	info->block_message = sem_open("block_message", O_CREAT, S_IRWXU | S_IRGRP | S_IROTH, 1);
	info->forks = sem_open("forks", O_CREAT, S_IRWXU | S_IRGRP | S_IROTH, info->numb_of_philo);
	lifetime(&info->start_time, &info->real_time, 1);
	return (1);
}

void	init_philos(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->numb_of_philo)
	{
		philo[i].info = info;
		philo[i].ate = 0;
		philo[i].num = i;
		i++;
	}
}
