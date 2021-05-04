#include "philo_two.h"

int	create_philo(t_philo *ph, int numb)
{
	int	i;
	int	count;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			lifetime(ph[i].info->block_time, &ph[i].life_time,
				&ph[i].curr_time, 1);
			if (pthread_create(&ph[i].philo, NULL, routine, &ph[i]))
				return (ternar_int(write(2, PTH_CREATE, 22) > 0, 1, 0));
			myusleep(100);
			i += 2;
		}
		if (count == 0)
			myusleep(ph[0].info->time_to_eat * 0.9 * 1000);
		count++;
	}
	return (0);
}

int	join_philo(t_philo *ph, int numb)
{
	int	i;

	i = 0;
	while (i < numb)
	{
		if (pthread_join(ph[i++].philo, NULL))
			return (ternar_int(write(2, PTH_JOIN, 31) > 0, 1, 0));
	}
	return (0);
}

int	destroy_info(t_info *info)
{
	sem_close(info->forks);
	sem_close(info->block_message);
	sem_close(info->block_data);
	sem_close(info->block_time);
	sem_unlink("forks");
	sem_unlink("block_message");
	sem_unlink("block_data");
	sem_unlink("block_time");
	return (0);
}
