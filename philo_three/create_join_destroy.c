#include "philo_three.h"

int	create_process_treads(t_philo *ph, t_info *info)
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
			ph[i].info->pids[i] = fork();
			if (!ph[i].info->pids[i])
			{
				lifetime(ph[i].info->block_time, &ph[i].life_time, &ph[i].curr_time, 1);
				if (pthread_create(&ph[i].philo, NULL, routine, &ph[i]))
					exit(ternar_int(write(2, PTH_CREATE, 33) > 0, 1, 0));
				myusleep(100);
				if (check_die(&ph[i], info) == 1)
					exit(1);
				else
				{
					pthread_join(ph[i].philo, NULL);
					exit(2);
				}
			}
			if (ph[i].info->pids[i] < 0)
				return (ternar_int(write(2, ERR_FORK, 23) > 0, 1, 0));
			i += 2;
		}
		if (count == 0)
			myusleep(ph[0].info->time_to_eat * 0.9 * 1000);
		count++;
	}
	i = 0;
	while (i < ph->info->numb_of_philo)
	{
		waitpid(-1, &status, WUNTRACED);
		if ((status = WEXITSTATUS(status)) == 1)
		{
			i = 0;
			while (i < info->numb_of_philo)
			{
				// printf("PRIV\n");
				kill(ph[i].info->pids[i], SIGKILL);
				i++;
			}
			return (1);
		}
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
