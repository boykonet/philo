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

void	philo_eat(t_philo *philo, int left_fork, int right_fork)
{
	if (philo->ate != -1)
	{
		pthread_mutex_lock(&philo->info->forks[left_fork]);
		print_message(philo, lifetime(&philo->info->start_time,
				&philo->info->real_time, 0), TAKEN_FORK, LEFT_FORK);
		pthread_mutex_lock(&philo->info->forks[right_fork]);
		print_message(philo, lifetime(&philo->info->start_time,
				&philo->info->real_time, 0), TAKEN_FORK, RIGHT_FORK);
		lifetime(&philo->start_time, &philo->real_time, 1);
		if (philo->info->died == 1)
			return ;
		print_message(philo, lifetime(&philo->info->start_time,
				&philo->info->real_time, 0), PHILO_EAT, NULL);
		myusleep(philo->info->time_to_eat * 1000);
		if (philo->info->must_eat != -1 && philo->ate != -1)
			philo->ate++;
		pthread_mutex_unlock(&philo->info->forks[right_fork]);
		print_message(philo, lifetime(&philo->info->start_time,
				&philo->info->real_time, 0), PUT_FORK, RIGHT_FORK);
		pthread_mutex_unlock(&philo->info->forks[left_fork]);
		print_message(philo, lifetime(&philo->info->start_time,
				&philo->info->real_time, 0), PUT_FORK, LEFT_FORK);
	}
}

void	*routine(void *philo)
{
	while (!((t_philo *)philo)->info->died)
	{
		if (!((t_philo *)philo)->info->died)
			philo_eat(philo, ((t_philo *)philo)->left_fork,
				((t_philo *)philo)->right_fork);
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
	return (NULL);
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
	int		status;

	status = 0;
	if (argc != 5 && argc != 6)
	{
		write(2, COUNT_PARAMS, 26);
		return (1);
	}
	if (!init_info(&info, argc, argv))
		return (1);
	philo = malloc(sizeof(t_philo) * info.numb_of_philo);
	if (!philo)
		return (0);
	init_philos(philo, &info);
	create_philo(philo, info.numb_of_philo);
	check_die(philo, &info);
	if (!join_philo(philo, info.numb_of_philo))
		status = 1;
	destroy_init(&info);
	free(philo);
	return (status);
}
