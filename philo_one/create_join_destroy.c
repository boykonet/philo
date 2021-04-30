#include "philo_one.h"

int	create_philo(t_philo *philo, int numb)
{
	int	i;
	int	count;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			lifetime(&philo[i].start_time, &philo[i].real_time, 1);
			if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
				return (ternar_int(write(2, PTH_CREATE, 22) > 0, 1, 0));
			i += 2;
		}
		if (count == 0)
			myusleep(philo[0].info->time_to_eat * 0.99 * 1000);
		count++;
	}
	return (0);
}

int	join_philo(t_philo *philo, int numb)
{
	int	i;

	i = 0;
	while (i < numb)
	{
		if (pthread_join(philo[i++].philo, NULL))
			return (1);
	}
	return (0);
}

int	destroy_info(t_info *info)
{
	int	i;
	int status;

	i = 0;
	status = 0;
	if (pthread_mutex_destroy(&info->block_message)
	|| pthread_mutex_destroy(&info->block_data))
	{
		write(2, PTH_M_DESTROY, 40);
		status = 1;
	}
	while (i < info->numb_of_philo)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
		{
			write(2, PTH_M_DESTROY, 40);
			status = 1;
		}
		i++;
	}
	free(info->forks);
	return (status);
}
