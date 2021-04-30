#include "philo_three.h"

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
			philo[i].info->pids[i] = fork();
			if (!philo[i].info->pids[i])
			{
				lifetime(&philo[i].start_time, &philo[i].real_time, 1);
				if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
					exit(ternar_int(write(2, PTHREAD_CREATE, 22) > 0, 1, 0));
				
			}
			else
				return (ternar_int(write(2, ERR_FORK, 13) > 0, 1, 0));
			i += 2;
		}
		if (count == 0)
			myusleep(500);
		count++;
	}
	waitpid(-1, &status, WUNTRACED);
	status = WEXITSTATUS(status);
	i = 0;
	if (status)
	{
		while (i < numb)
		{
			kill(philo[i].info->pids[i], SIGKILL);
			i++;
		}
	}
	return (0);
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

void	destroy_info(t_info *info)
{
	sem_close(info->block_message);
	sem_close(info->forks);
	sem_unlink("forks");
	sem_unlink("block_message");
}
