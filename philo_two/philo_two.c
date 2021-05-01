/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarina <gkarina@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 00:26:42 by gkarina           #+#    #+#             */
/*   Updated: 2021/05/01 23:36:50 by gkarina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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
		sem_wait(ph->info->block_data);
		ph->info->must_eat++;
		sem_post(ph->info->block_data);
		return (2);
	}
	return (0);
}

void	*routine(void *ph)
{
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

void	check_die(t_philo *ph, t_info *info)
{
	int	i;

	while (TRUE)
	{
		i = 0;
		while (i < info->numb_of_philo)
		{
			if (lifetime(ph[i].info->block_time, &ph[i].life_time,
					&ph[i].curr_time, 0) >= info->time_to_die)
			{
				message(&ph[i], lifetime(ph[i].info->block_time,
						&info->start_time, &info->curr_time, 0), PH_DIED, NULL);
				return ;
			}
			if (info->must_eat == info->numb_of_philo)
			{
				sem_wait(info->block_data);
				info->philos_eat = 1;
				sem_post(info->block_data);
				return ;
			}
			i++;
		}
		myusleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*ph;
	int		status;

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
	if (!status && create_philo(ph, info.numb_of_philo))
		status = 1;
	if (!status)
		check_die(ph, &info);
	if (!status && join_philo(ph, info.numb_of_philo))
		status = 1;
	destroy_info(&info);
	free(ph);
	return (status);
}
