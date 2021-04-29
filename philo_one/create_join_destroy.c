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
			if (lifetime(&philo[i].start_time, &philo[i].real_time, 1) == -1)
				return (1);
			if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
				return (ternar_int(write(2, PTH_CREATE, 22) > 0, 1, 0));
			i += 2;
		}
		if (count == 0)
			if (myusleep(500) == -1)
				return (1);
		count++;
	}
	return (0);
}

int	join_philo(t_philo *philo, int numb)
{
	int		i;
	int		count;
	void	*ptr;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			if (pthread_join(philo[i].philo, &ptr))
				return (ternar_int(write(2, PTH_JOIN, 20) > 0, 1, 0));
			if (!ptr)
				return (1);
			i += 2;
		}
		count++;
	}
	return (0);
}

int	destroy_info(t_info *info)
{
	int	i;
	int status;

	i = 0;
	status = 0;
//	if (pthread_mutex_unlock(&info->block_message))
//		printf("cho delaesh???\n");
	pthread_mutex_destroy(&info->valera);
	if (pthread_mutex_destroy(&info->block_message))
	{
		write(2, PTH_M_DESTROY, 40);
		status = 1;
	}
	while (i < info->numb_of_philo)
	{
		if (pthread_mutex_destroy(&info->forks[i++]))
		{
//			printf("PRIVETIKI\n");
			write(2, PTH_M_DESTROY, 40);
			status = 1;
		}
	}
	free(info->forks);
	return (status);
}
