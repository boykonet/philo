/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarina <gkarina@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 00:26:42 by gkarina           #+#    #+#             */
/*   Updated: 2021/04/27 20:38:34 by gkarina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	philo_eat(t_philo *ph)
{
	sem_wait(ph->info->forks);
	message(ph, lifetime(ph->info->block_time, &ph->info->start_time,
			&ph->info->curr_time, 0), TAKEN_FORK, LEFT_FORK);
	sem_wait(ph->info->forks);
	message(ph, lifetime(ph->info->block_time, &ph->info->start_time,
			&ph->info->curr_time, 0), TAKEN_FORK, RIGHT_FORK);
	lifetime(ph->info->block_time, &ph->life_time,
		  &ph->curr_time, 1);
	message(ph, lifetime(ph->info->block_time, &ph->info->start_time,
			&ph->info->curr_time, 0), PH_EAT, NULL);
	myusleep(ph->info->time_to_eat * 1000);
	if (ph->ate != -1)
		ph->ate++;
	sem_post(ph->info->forks);
	sem_post(ph->info->forks);
	if (ph->ate == ph->info->numb_must_eat)
	{
		/* sem_wait(ph->info->block_data); */
		ph->info->must_eat++;
		/* sem_post(ph->info->block_data); */
		return (2);
	}
	return (0);
}

void	*routine(void *ph)
{
	pthread_detach(((t_philo *)ph)->philo);
	while (!((t_philo *)ph)->info->died
		&& !((t_philo *)ph)->info->philos_eat)
	{
		if (!((t_philo *)ph)->info->died)
		{
			if (philo_eat(ph) == 2)
				break ;
		}
		if (!((t_philo *)ph)->info->died && !((t_philo *)ph)->info->philos_eat)
		{
			message(ph, lifetime(((t_philo *)ph)->info->block_time,
					&((t_philo *)ph)->info->start_time,
					&((t_philo *)ph)->info->curr_time, 0), PH_SLEEP, NULL);
			myusleep(((t_philo *)ph)->info->time_to_sleep * 1000);
		}
		if (!((t_philo *)ph)->info->died && !((t_philo *)ph)->info->philos_eat)
			message(ph, lifetime(((t_philo *)ph)->info->block_time,
					&((t_philo *)ph)->info->start_time,
					&((t_philo *)ph)->info->curr_time, 0), PH_THINK, NULL);
	}
	return (NULL);
}

int	check_die(t_philo *ph, t_info *info)
{
	while (TRUE)
	{
		if (lifetime(ph->info->block_time, &ph->life_time,
				&ph->curr_time, 0) >= info->time_to_die)
		{
			message(ph, lifetime(ph->info->block_time,
					&info->start_time, &info->curr_time, 0), PH_DIED, NULL);
			return (1);
		}
		if (info->must_eat > 0)
			return (2);
		myusleep(1000);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*ph;
	int 	status;

	status = 0;
	if (argc != 5 && argc != 6)
		return (ternar_int(write(2, COUNT_PARAMS, 37) > 0, 1, 0));
	if (init_info(&info, argc, argv))
		return (1);
	ph = malloc(sizeof(t_philo) * info.numb_of_philo);
	if (!ph)
		status = 1;
	if (!status)
		init_philos(ph, &info);
	if (!status && create_process_treads(ph, &info))
		status = 1;
	// if (!status)
	// 	check_die(philo, &info);
	// if (!status && join_philo(philo, info.numb_of_philo))
	// 	status = 1;
	destroy_info(&info);
	free(ph);
	return (status);
}
