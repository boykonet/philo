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
	pthread_mutex_lock(&philo->info->forks[left_fork]);
	print_message(philo, lifetime(&philo->info->block_time,
			&philo->info->start_time, &philo->info->curr_time,
			0), TAKEN_FORK, LEFT_FORK);

	pthread_mutex_lock(&philo->info->forks[right_fork]);
	print_message(philo, lifetime(&philo->info->block_time,
			&philo->info->start_time, &philo->info->curr_time,
			0), TAKEN_FORK, RIGHT_FORK);

	lifetime(&philo->info->block_time, &philo->life_time,
		  &philo->curr_time, 1);

	print_message(philo, lifetime(&philo->info->block_time,
			&philo->info->start_time, &philo->info->curr_time,
			0), PHILO_EAT, NULL);

	myusleep(philo->info->time_to_eat * 1000);

	if (philo->ate != -1)
		philo->ate++;

	pthread_mutex_unlock(&philo->info->forks[right_fork]);

	pthread_mutex_unlock(&philo->info->forks[left_fork]);

	if (philo->ate == philo->info->numb_must_eat)
	{
		pthread_mutex_lock(&philo->info->block_data);
		philo->info->must_eat++;
		pthread_mutex_unlock(&philo->info->block_data);
		return (2);
	}

	return (0);
}

void	*routine(void *philo)
{
	while (!((t_philo *)philo)->info->died && !((t_philo *)philo)->info->philos_eat)
	{
		if (!((t_philo *)philo)->info->died)
		{
			if (philo_eat(philo, ((t_philo *)philo)->left_fork,
					((t_philo *)philo)->right_fork) == 2)
				break ;
		}
		if (!((t_philo *)philo)->info->died && !((t_philo *)philo)->info->philos_eat)
		{
			print_message(philo, lifetime(&((t_philo *)philo)->info->block_time,
					&((t_philo *)philo)->info->start_time,
					&((t_philo *)philo)->info->curr_time, 0), PHILO_SLEEP, NULL);
			myusleep(((t_philo *)philo)->info->time_to_sleep * 1000);
		}
		if (!((t_philo *)philo)->info->died && !((t_philo *)philo)->info->philos_eat)
			print_message(philo, lifetime(&((t_philo *)philo)->info->block_time,
					&((t_philo *)philo)->info->start_time,
					&((t_philo *)philo)->info->curr_time, 0), PHILO_THINK, NULL);
	}
	return (NULL);
}

void	check_die(t_philo *p, t_info *info)
{
	int	i;
	long	num;
	int 	j;

	j = 0;
	while (TRUE)
	{
		i = 0;
		while (i < info->numb_of_philo)
		{
			if ((num = lifetime(&p[i].info->block_time, &p[i].life_time,
					   &p[i].curr_time, 0)) >= info->time_to_die)
			{
				printf("%d - num = %ld\n", j, num);
				print_message(&p[i], lifetime(&p[i].info->block_time,
						&info->start_time, &info->curr_time, 0), PHILO_DIED, NULL);
				return ;
			}
			if (info->must_eat == info->numb_of_philo)
			{
				pthread_mutex_lock(&info->block_data);
				info->philos_eat = 1;
				pthread_mutex_unlock(&info->block_data);
				return ;
			}
			i++;
			j++;
		}
		myusleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*philo;
	int 	status;

	status = 0;
	if (argc != 5 && argc != 6)
		return (ternar_int(write(2, COUNT_PARAMS, 37) > 0, 1, 0));
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
	if (!status && join_philo(philo, info.numb_of_philo))
		status = 1;
	destroy_info(&info);
	free(philo);
	return (status);
}
