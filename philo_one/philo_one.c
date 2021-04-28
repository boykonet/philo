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

#include "philo_one.h"

int	philo_eat(t_philo *philo, int left_fork, int right_fork)
{
	if (pthread_mutex_lock(&philo->info->forks[left_fork]))
		return (ternar_int(write(2, PTH_M_LOCK, 37) > 0, 1, 0));
	print_message(philo, lifetime(&philo->info->start_time,
			&philo->info->real_time, 0), TAKEN_FORK, LEFT_FORK);

	if (pthread_mutex_lock(&philo->info->forks[right_fork]))
		return (ternar_int(write(2, PTH_M_LOCK, 37) > 0, 1, 0));
	print_message(philo, lifetime(&philo->info->start_time,
			&philo->info->real_time, 0), TAKEN_FORK, RIGHT_FORK);

	lifetime(&philo->start_time, &philo->real_time, 1);

	print_message(philo, lifetime(&philo->info->start_time,
			&philo->info->real_time, 0), PHILO_EAT, NULL);

	myusleep(philo->info->time_to_eat * 1000);

	if (philo->info->must_eat != -1 && philo->ate != -1)
		philo->ate++;

	if (pthread_mutex_unlock(&philo->info->forks[right_fork]))
		return (ternar_int(write(2, PTH_M_UNLOCK, 39) > 0, 1, 0));
	print_message(philo, lifetime(&philo->info->start_time,
			&philo->info->real_time, 0), PUT_FORK, RIGHT_FORK);
	if (pthread_mutex_unlock(&philo->info->forks[left_fork]))

		return (ternar_int(write(2, PTH_M_UNLOCK, 39) > 0, 1, 0));
	print_message(philo, lifetime(&philo->info->start_time,
			&philo->info->real_time, 0), PUT_FORK, LEFT_FORK);
	return (0);
}

void	*routine(void *philo)
{
	while (!((t_philo *)philo)->info->died)
	{
		if (!((t_philo *)philo)->info->died)
			if (philo_eat(philo, ((t_philo *)philo)->left_fork,
				((t_philo *)philo)->right_fork))
				return (NULL);
		if (((t_philo *)philo)->info->must_eat != -1
			&& ((t_philo *)philo)->ate == -1)
			break ;
		if (!((t_philo *)philo)->info->died)
		{
			print_message(philo, lifetime(&((t_philo *)philo)->info->start_time,
					&((t_philo *)philo)->info->real_time, 0),
				 	PHILO_SLEEP, NULL);
			myusleep(((t_philo *)philo)->info->time_to_sleep * 1000);
		}
		if (!((t_philo *)philo)->info->died)
			print_message(philo, lifetime(&((t_philo *)philo)->info->start_time,
					&((t_philo *)philo)->info->real_time, 0),
				 	PHILO_THINK, NULL);
	}
	return (philo);
}

void	check_die(t_philo *p, t_info *info)
{
	int	i;

	while (TRUE)
	{
		i = 0;
		while (i < info->numb_of_philo)
		{
			if (lifetime(&p[i].start_time, &p[i].real_time, 0)
				>= info->time_to_die)
			{
				print_message(&p[i], lifetime(&info->start_time,
						&info->real_time, 0), PHILO_DIED, NULL);
				info->died = 1;
				return ;
			}
			if (info->must_eat != -1 && (p[i].ate == info->numb_must_eat))
			{
				p[i].ate = -1;
				info->must_eat++;
				if (info->must_eat == info->numb_of_philo)
				{
					myusleep(500);
					return ;
				}
			}
			i++;
		}
	}
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*philo;
	int 	status;

	status = 0;
	if (argc != 5 && argc != 6)
	{
		write(2, COUNT_PARAMS, 37);
		return (1);
	}
	if (init_info(&info, argc, argv))
		return (1);
	philo = malloc(sizeof(t_philo) * info.numb_of_philo);
	if (!philo)
		status = 1;
	if (!status)
		init_philos(philo, &info);
	if (!status && create_philo(philo, info.numb_of_philo))
		status = 1;
	if (!status)
		check_die(philo, &info);
	if (join_philo(philo, info.numb_of_philo))
		status = 1;
	destroy_info(&info);
	free(philo);
	return (status);
}
