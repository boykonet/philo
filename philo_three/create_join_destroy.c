#include "philo_three.h"

int	dother_proc_tread(t_philo *ph, t_info *info, int i)
{
	while (i < info->numb_of_philo)
	{
		ph[i].info->pids[i] = fork();
		if (ph[i].info->pids[i] == 0)
		{
			lifetime(ph[i].info->block_time, &ph[i].life_time,
				&ph[i].curr_time, 1);
			if (pthread_create(&ph[i].philo, NULL, routine, &ph[i]))
				exit(ternar_int(write(2, PTH_CREATE, 33) > 0, 1, 0));
			pthread_detach(((t_philo *)ph)->philo);
			myusleep(100);
			if (check_die(&ph[i], info) == 1)
				exit(1);
			else
				exit(2);
		}
		if (ph[i].info->pids[i] < 0)
			return (ternar_int(write(2, ERR_FORK, 23) > 0, 1, 0));
		i += 2;
	}
	return (0);
}

int	parent_waiter(t_philo *ph, t_info *info)
{
	int	i;
	int	status;

	i = 0;
	while (i < ph->info->numb_of_philo)
	{
		if (waitpid(-1, &status, WUNTRACED) == -1)
			return (ternar_int(write(2, ERR_WAITPID, 26) > 0, 1, 0));
		status = WEXITSTATUS(status);
		if (status == 1)
		{
			i = 0;
			while (i < info->numb_of_philo)
			{
				kill(ph[i].info->pids[i], SIGKILL);
				i++;
			}
			return (2);
		}
		else if (status == 2)
			i++;
	}
	return (0);
}

int	create_process_treads(t_philo *ph, t_info *info)
{
	int	i;
	int	count;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		if (dother_proc_tread(ph, info, i) == 1)
			return (1);
		if (count == 0)
			myusleep(ph[0].info->time_to_eat * 0.9 * 1000);
		count++;
	}
	i = 0;
	if (parent_waiter(ph, info) == 1)
		return (1);
	return (0);
}

int	destroy_info(t_info *info)
{
	sem_close(info->forks);
	sem_close(info->block_message);
	sem_close(info->block_time);
	sem_unlink("forks");
	sem_unlink("block_message");
	sem_unlink("block_time");
	return (0);
}
