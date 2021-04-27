#include "philo_one.h"

int	create_philo(t_philo *philo, int numb)
{
	int	i;
	int	status;
	int	count;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			lifetime(&philo[i].start_time, &philo[i].real_time, 1);
			status = pthread_create(&philo[i].philo, NULL, routine, &philo[i]);
			if (status)
			{
				write(2, PTHREAD_CREATE, 22);
				return (0);
			}
			i += 2;
		}
		if (count == 0)
			myusleep(500);
		count++;
	}
	return (1);
}

int	join_philo(t_philo *philo, int numb)
{
	int		i;
	int		count;
	int		status;
	void	*ptr;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			status = pthread_join(philo[i].philo, &ptr);
			if (status)
			{
				write(2, PTHREAD_JOIN, 20);
				return (0);
			}
			i += 2;
		}
		count++;
	}
	return (1);
}

void	destroy_init(t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&info->block_message);
	pthread_mutex_destroy(&info->increment_eat);
	while (i < info->numb_of_philo)
		pthread_mutex_destroy(&info->forks[i++]);
	free(info->forks);
}
