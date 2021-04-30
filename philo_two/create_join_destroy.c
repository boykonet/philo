#include "philo_two.h"

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

void	destroy_info(t_info *info)
{
	sem_close(info->block_message);
	sem_close(info->block_data);
	sem_close(info->forks);
	sem_unlink("forks");
	sem_unlink("block_message");
	sem_unlink("data");
}
