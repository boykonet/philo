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

void					philo_eat(t_philo *philo, int left_fork, int right_fork)
{
	pthread_mutex_lock(&philo->info->forks[left_fork]);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), TAKEN_FORK, LEFT_FORK);

	pthread_mutex_lock(&philo->info->forks[right_fork]);
	print_message(philo,lifetime(&philo->info->start_time, &philo->info->real_time, 0), TAKEN_FORK, RIGHT_FORK);

	lifetime(&philo->start_time, &philo->real_time, 1);

	philo->ate = 0;
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PHILO_EAT, NULL);
	myusleep(philo->info->time_to_eat * 1000);

	if (philo->info->must_eat != -1)
		philo->ate = 1;

	pthread_mutex_unlock(&philo->info->forks[right_fork]);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, RIGHT_FORK);

	pthread_mutex_unlock(&philo->info->forks[left_fork]);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, LEFT_FORK);

}

void					*routine(void *philo)
{
//	pthread_detach(((t_philo*)philo)->philo);
	while (!((t_philo*)philo)->info->died && !((t_philo*)philo)->info->philos_ate)
	{
		if (!((t_philo*)philo)->info->died)
			philo_eat(philo, ((t_philo*)philo)->left_fork, ((t_philo*)philo)->right_fork);

		if (((t_philo*)philo)->info->philos_ate)
			break ;

		if (!((t_philo*)philo)->info->died)
		{
			print_message(philo, lifetime(&((t_philo*)philo)->info->start_time, &((t_philo*)philo)->info->real_time, 0), PHILO_SLEEP, NULL);
			myusleep(((t_philo*)philo)->info->time_to_sleep * 1000);
		}

		if (!((t_philo*)philo)->info->died)
			print_message(philo, lifetime(&((t_philo*)philo)->info->start_time, &((t_philo*)philo)->info->real_time, 0), PHILO_THINK, NULL);

	}
	return (NULL);
}

void 					init_forks(pthread_mutex_t *forks, int numb)
{
	int					i;

	i = 0;
	while (i < numb)
		pthread_mutex_init(&forks[i++], NULL);
}

int						params_for_philo(char *str, int *data)
{
	void 				*endptr;

	*data = ft_strtoi(str, &endptr);
	if (*(char*)endptr)
		return (0);
	return (1);
}

int						init_info(t_info *info, int argc, char **argv)
{
	info->must_eat = ternar_int((argc == 6), 0, -1);
	pthread_mutex_init(&info->block, NULL);
	if (!params_for_philo(argv[1], &info->numb_of_philo))
		return (0);
	if (!params_for_philo(argv[2], &info->time_to_die))
		return (0);
	if (!params_for_philo(argv[3], &info->time_to_eat))
		return (0);
	if (!params_for_philo(argv[4], &info->time_to_sleep))
		return (0);
	info->numb_must_eat = 0;
	if (!info->must_eat)
		if (!params_for_philo(argv[5], &info->numb_must_eat))
			return (0);

	if (info->numb_of_philo < 0 || info->time_to_die < 0 || info->time_to_eat < 0
	|| info->time_to_sleep < 0 || (!info->must_eat && info->numb_must_eat < 0))
	{
		write(2, INCORRECT_PARAMS, 26);
		return (0);
	}
	info->philos_ate = 0;
	info->died = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->numb_of_philo);
	if (!info->forks)
		return (0);
	init_forks(info->forks, info->numb_of_philo);
	lifetime(&info->start_time, &info->real_time, 1);
	return (1);
}

void					init_philos(t_philo *philo, t_info *info)
{
	int		i;

	i = 0;
	while (i < info->numb_of_philo)
	{
		philo[i].info = info;
		philo[i].left_fork = (i + 1) % philo[i].info->numb_of_philo;
		philo[i].right_fork = i % philo[i].info->numb_of_philo;
		philo[i].ate = 0;
		philo[i].num = i;
		i++;
	}
}

int 					join_philo(t_philo *philo, int numb)
{
	int 				i;
	int 				count = 0;
	int 				status;
	void 				*ptr;

	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			lifetime(&philo[i].start_time, &philo[i].real_time, 1);
			status = pthread_join(philo[i].philo, &ptr);
			if (status)
			{
				write(2, "Error: pthread_join\n", 20);
				return (0);
			}
			i += 2;
		}
		count++;
	}
	return (1);
}

int 					create_philo(t_philo *philo, int numb)
{
	int 				i;
	int 				status;
	int 				count;

	count = 0;
	while (count < 2)
	{
		i = ternar_int(count == 0, 0, 1);
		while (i < numb)
		{
			lifetime(&philo[i].start_time, &philo[i].real_time, 1);
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
	return (1);
}

void					destroy_init(t_info *info)
{
	int 				i;

	i = 0;
	pthread_mutex_destroy(&info->block);
	while (i < info->numb_of_philo)
		pthread_mutex_destroy(&info->forks[i++]);
	free(info->forks);
}

int 					check_die(t_philo *philo, t_info *info)
{
	int 				i;
	int 				count_eat;
	int 				flag;

	flag = 0;
	while (!info->died && !info->philos_ate)
	{
		i = 0;
		count_eat = 0;
		while (i < info->numb_of_philo)
		{
			if (lifetime(&philo[i].start_time, &philo[i].real_time, 0) >= philo[i].info->time_to_die)
			{
				philo[i].info->died = 1;
				print_message(&philo[i], lifetime(&philo[i].info->start_time, &philo[i].info->real_time, 0), PHILO_DIED, NULL);
				return (0);
			}
			if (philo[i].info->must_eat != -1 && philo[i].ate == 1)
			{
				philo[i].ate = 0;
				count_eat++;
			}
			if (philo[i].info->must_eat != -1 && count_eat == info->numb_of_philo)
			{
				count_eat = 0;
				philo[i].info->must_eat++;
			}
			if (philo[i].info->must_eat != -1 && philo[i].info->must_eat == philo[i].info->numb_must_eat)
			{
				philo[i].info->philos_ate = 1;
				flag = 1;
				break ;
			}
			i++;
		}
		if (flag == 1)
			break ;
	}
	return (1);
}

int						main(int argc, char **argv)
{
	t_info				info;
	t_philo				*philo;
	int 				status;

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