#include "philo_three.h"

int	create_process_treads(t_philo *philo, t_info *info)
{
	int	i;
	int	status;
	int	count;

	// i = 0;
	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < info->numb_of_philo)
		{
			philo[i].info->pids[i] = fork();
			if (!philo[i].info->pids[i])
			{
				lifetime(&philo[i].start_time, &philo[i].real_time, 1);
				if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
					exit(ternar_int(write(2, PTH_CREATE, 22) > 0, 1, 0));
				myusleep(info->time_to_eat * 0.99 * 1000);
				check_die(&philo[i], info);
				pthread_join(philo[i].philo, NULL);
				exit(1);
			}
			if (philo[i].info->pids[i] < 0)
				return (ternar_int(write(2, ERR_FORK, 23) > 0, 1, 0));
			i += 2;
		}
		if (count == 0)
			myusleep(philo[0].info->time_to_eat * 0.99 * 1000);
		count++;
	}
	waitpid(-1, &status, WUNTRACED);
	status = WEXITSTATUS(status);
	i = 0;
	while (i < info->numb_of_philo)
	{
		kill(philo[i].info->pids[i], SIGKILL);
		i++;
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
