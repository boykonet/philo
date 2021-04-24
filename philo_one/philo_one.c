/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarina <gkarina@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 00:26:42 by gkarina           #+#    #+#             */
/*   Updated: 2021/04/22 23:04:35 by gkarina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int						philo_eat(t_philo *philo, int left_fork, int right_fork)
{
	long 				num;

	pthread_mutex_lock(&philo->info->forks[left_fork]);
	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, TAKEN_FORK, LEFT_FORK);

	pthread_mutex_lock(&philo->info->forks[right_fork]);
	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, TAKEN_FORK, RIGHT_FORK);

//	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PHILO_EAT, NULL);

	if (philo->life_cycle + philo->info->time_to_eat >= philo->info->time_to_die)
	{
		philo->info->died = 1;
		return (0);
	}
	myusleep(philo->info->time_to_eat * 1000);
	philo->life_cycle = lifetime(&philo->start_time, &philo->real_time, 1);
	philo->ate++;

	pthread_mutex_unlock(&philo->info->forks[right_fork]);
//	num = lifetime(&philo->info->start_time, &philo->info->real_time);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, RIGHT_FORK);

	pthread_mutex_unlock(&philo->info->forks[left_fork]);
//	num = lifetime(&philo->info->start_time, &philo->info->real_time);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, LEFT_FORK);

	return (1);
}

int						philo_sleep(t_philo *philo)
{
	long 				num;

	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, PHILO_SLEEP, NULL);

	if (philo->life_cycle + philo->info->time_to_sleep >= philo->info->time_to_die)
	{
		philo->info->died = 1;
		return (0);
	}
	myusleep(philo->info->time_to_sleep * 1000);
//	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	philo->life_cycle = lifetime(&philo->info->start_time, &philo->info->real_time, 1);

	return (1);
}

int						philo_think(t_philo *philo)
{
	long 				num;

	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, PHILO_THINK, NULL);

	philo->life_cycle = lifetime(&philo->info->start_time, &philo->info->real_time, 1);
	if (philo->life_cycle >= philo->info->time_to_die)
	{
		philo->info->died = 1;
		return (0);
	}

	return (1);
}

void					*routine(void *philo)
{
	int 				status;

	printf("START %ld %ld %d\n", ((t_philo*)philo)->life_cycle, ((t_philo*)philo)->start_time.tv_sec, ((t_philo*)philo)->start_time.tv_usec);
	((t_philo*)philo)->start_time.tv_sec = 0;
	((t_philo*)philo)->start_time.tv_usec = 0;
	((t_philo*)philo)->life_cycle = 0;
	printf("END %ld %ld %d\n", ((t_philo*)philo)->life_cycle, ((t_philo*)philo)->start_time.tv_sec, ((t_philo*)philo)->start_time.tv_usec);
	status = philo_eat(philo, ((t_philo*)philo)->left_fork, ((t_philo*)philo)->right_fork);
	if (!status && ((t_philo*)philo)->info->died == 1)
		return (NULL);

	status = philo_sleep(philo);
	if (!status && ((t_philo*)philo)->info->died == 1)
		return (NULL);

	status = philo_think(philo);
	if (!status && ((t_philo*)philo)->info->died == 1)
		return (NULL);
	return (philo);
}

void 					init_forks(pthread_mutex_t *forks, int numb)
{
	int					i;

	i = 0;
	while (i < numb)
		pthread_mutex_init(&forks[i++], NULL);
}

t_info					*init_info(t_info *info, char **argv)
{
	pthread_mutex_init(&info->block, NULL);
	info->numb_of_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->numb_must_eat = ternar_int((argv[5] && !info->must_eat), \
	ft_atoi(argv[5]), 0);

	if (info->numb_of_philo < 0 || info->time_to_die < 0 || info->time_to_eat < 0
	|| info->time_to_sleep < 0 || info->numb_must_eat < 0)
	{
		write(2, CORRECT_PARAMS, 26);
		return (NULL);
	}
	info->died = 0;
	info->start_time.tv_sec = 0;
	info->start_time.tv_usec = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->numb_of_philo);
	if (!info->forks)
		return (NULL);
	init_forks(info->forks, info->numb_of_philo);
	return (info);
}

void					init_philos(t_philo *philo, t_info *info)
{
	int		i;

	i = 0;
	while (i < info->numb_of_philo)
	{
		philo[i].info = info;
		philo[i].life_cycle = 0;
		philo[i].left_fork = (i + 1) % philo[i].info->numb_of_philo;
		philo[i].right_fork = i % philo[i].info->numb_of_philo;
		philo[i].num = i;
		philo[i].ate = 0;
		i++;
	}
}

int 					create_and_join_philo(t_philo *philo, int numb)
{
	int 				i;
	int 				status;
	int 				count;
	void 				*ptr;

	count = 0;
	while (count < 2)
	{
		if (count == 0)
			i = 0;
		else
			i = 1;
		while (i < numb)
		{
			status = pthread_create(&philo[i].philo, NULL, routine, &philo[i]);
			if (status)
			{
				write(2, "Error: pthread_create\n", 22);
				return (0);
			}
			i += 2;
		}
		if (count == 0)
			myusleep(100);
		count++;
	}
	i = 0;
	while (i < numb)
	{
		status = pthread_join(philo[i].philo, &ptr);
		if (status)
		{
			write(2, "Error: pthread_join\n", 20);
			return (0);
		}
		if (!ptr)

		i++;
	}
	return (1);
}

void					destroy_init(t_info *info)
{
	int 				i;

	i = 0;
	pthread_mutex_destroy(&info->block);
	while (i < info->numb_of_philo)
		pthread_mutex_destroy(&info->forks[i]);
	free(info->forks);
}

int						main(int argc, char **argv)
{
	t_info				info;
	t_philo				*philo;

	if (argc == 5 || argc == 6)
	{
		info.must_eat = ternar_int((argc == 6), 0, -1);
		if (!init_info(&info, argv))
			return (-1);
		philo = malloc(sizeof(t_philo) * info.numb_of_philo);
		if (!philo)
			return (0);
		init_philos(philo, &info);
		while (TRUE)
		{
			create_and_join_philo(philo, info.numb_of_philo);
			if (info.must_eat == info.numb_must_eat)
				break ;
			if (philo->ate == philo->info->numb_must_eat
				&& philo->info->must_eat != -1)
				break ;
		}
		free(philo);
		destroy_init(&info);
	}
	else
		write(2, COUNT_PARAMS, 26);
	return (0);
}