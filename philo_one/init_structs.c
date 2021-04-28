#include "philo_one.h"

static int	init_mutex(pthread_mutex_t *mutex, int numb)
{
	int	i;

	i = 0;
	while (i < numb)
	{
		if (pthread_mutex_init(&mutex[i++], NULL))
			return (ternar_int(write(2, PTH_M_INIT, 26) > 0, 1, 0));
	}
	return (0);
}

static int	params_for_philo(char *str, int *data)
{
	void	*endptr;

	*data = ft_strtoi(str, &endptr);
	if (*(char *)endptr)
		return (ternar_int(write(2, INCORRECT_PARAMS, 26) > 0, 1, 0));
	return (0);
}

int	init_info(t_info *info, int argc, char **argv)
{
	info->must_eat = ternar_int((argc == 6), 0, -1);
	if (params_for_philo(argv[1], &info->numb_of_philo))
		return (1);
	if (params_for_philo(argv[2], &info->time_to_die))
		return (1);
	if (params_for_philo(argv[3], &info->time_to_eat))
		return (1);
	if (params_for_philo(argv[4], &info->time_to_sleep))
		return (1);
	info->numb_must_eat = 0;
	if (!info->must_eat)
		if (params_for_philo(argv[5], &info->numb_must_eat))
			return (1);
	if (info->numb_of_philo <= 2 || info->numb_of_philo > 200
		|| info->time_to_die <= 0 || info->time_to_eat <= 0
		|| info->time_to_sleep <= 0 || (!info->must_eat && info->numb_must_eat <= 0))
		return (ternar_int(write(2, INCORRECT_PARAMS, 37) > 0, 1, 0));
	info->died = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->numb_of_philo);
	if (!info->forks)
		return (1);
	if (init_mutex(info->forks, info->numb_of_philo))
		return (1);
	if (init_mutex(&info->block_message, 1))
		return (1);
	lifetime(&info->start_time, &info->real_time, 1);
	return (0);
}

void	init_philos(t_philo *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->numb_of_philo)
	{
		philo[i].info = info;
		philo[i].left_fork = (i + 1) % philo[i].info->numb_of_philo;
		philo[i].right_fork = i % philo[i].info->numb_of_philo;
		philo[i].ate = 0;
		philo[i].num = i;
		i++;
	}
}
