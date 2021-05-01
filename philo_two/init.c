#include "philo_two.h"

sem_t	*create_new_sem(const char *name, int count)
{
	sem_t	*new_sem;

	sem_unlink(name);
	new_sem = sem_open(name, O_CREAT, S_IRWXU | S_IRGRP | S_IROTH, count);
	if (new_sem == SEM_FAILED)
		return (NULL);
	return (new_sem);
}

static int	params_for_philo(char *str, int *data)
{
	void	*endptr;

	*data = ft_strtoi(str, &endptr);
	if (*(char *)endptr)
		return (ternar_int(write(2, INCORRECT_PARAMS, 26) > 0, 1, 0));
	return (0);
}

int	correct_argv(t_info *info, int argc, char **argv)
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
		|| info->time_to_sleep <= 0
		|| (!info->must_eat && info->numb_must_eat <= 0))
		return (ternar_int(write(2, INCORRECT_PARAMS, 37) > 0, 1, 0));
	return (0);
}

int	init_info(t_info *info, int argc, char **argv)
{
	if (correct_argv(info, argc, argv))
		return (1);
	info->died = 0;
	info->philos_eat = 0;
	info->forks = create_new_sem("forks", info->numb_of_philo);
	info->block_message = create_new_sem("block_message", 1);
	info->block_data = create_new_sem("block_data", 1);
	info->block_time = create_new_sem("block_time", 1);
	if (!info->block_message || !info->forks
		|| !info->block_data || !info->block_time)
	{
		return (1);
	}
	lifetime(info->block_time, &info->start_time, &info->curr_time, 1);
	return (0);
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
