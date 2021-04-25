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

//	printf("%d - start time: %ld %d\n", philo->num + 1, philo->start_time.tv_sec, philo->start_time.tv_usec);
//	printf("%d = real time: %ld %d\n", philo->num + 1, philo->real_time.tv_sec, philo->real_time.tv_usec);
//	printf("%d = time = %ld\n", philo->num + 1, lifetime(&philo->start_time, &philo->real_time, 0));

	pthread_mutex_lock(&philo->info->forks[left_fork]);
	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, TAKEN_FORK, LEFT_FORK);

	pthread_mutex_lock(&philo->info->forks[right_fork]);
	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, TAKEN_FORK, RIGHT_FORK);

	if (lifetime(&philo->start_time, &philo->real_time, 0) >= philo->info->time_to_die)
	{
		printf("start time: %ld %d\n", philo->start_time.tv_sec, philo->start_time.tv_usec);
		printf("real time: %ld %d\n", philo->real_time.tv_sec, philo->real_time.tv_usec);
		printf("time = %ld\n", lifetime(&philo->start_time, &philo->real_time, 0) + philo->info->time_to_eat);
		printf("%d - privet\n", philo->num + 1);
		philo->died = 1;
		philo->info->died = 1;
		return (0);
	}
	lifetime(&philo->start_time, &philo->real_time, 1);

	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PHILO_EAT, NULL);

	myusleep(philo->info->time_to_eat * 1000);
	if (philo->info->died)
		return (0);

	philo->ate++;

	pthread_mutex_unlock(&philo->info->forks[right_fork]);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, RIGHT_FORK);

	pthread_mutex_unlock(&philo->info->forks[left_fork]);
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PUT_FORK, LEFT_FORK);

	return (1);
}

int						philo_sleep(t_philo *philo)
{
	long 				num;

	num = lifetime(&philo->info->start_time, &philo->info->real_time, 0);
	print_message(philo, num, PHILO_SLEEP, NULL);

	if (lifetime(&philo->start_time, &philo->real_time, 0) + philo->info->time_to_sleep >= philo->info->time_to_die)
	{
		philo->info->died = 1;
		return (0);
	}
	myusleep(philo->info->time_to_sleep * 1000);

	if (philo->info->died)
		return (0);

	return (1);
}

int						philo_think(t_philo *philo)
{
	print_message(philo, lifetime(&philo->info->start_time, &philo->info->real_time, 0), PHILO_THINK, NULL);

	if (lifetime(&philo->start_time, &philo->real_time, 0) >= philo->info->time_to_die)
	{
		philo->info->died = 1;
		return (0);
	}

	return (1);
}

void					*routine(void *philo)
{
	int 				status;

//	pthread_mutex_lock(&((t_philo*)philo)->info->block);
//	printf("%d - START %ld\n", ((t_philo*)philo)->num + 1, lifetime(&((t_philo*)philo)->start_time, &((t_philo*)philo)->real_time));
//	pthread_mutex_unlock(&((t_philo*)philo)->info->block);

//	pthread_mutex_lock(&((t_philo*)philo)->info->block);
//	printf("%d - END %ld\n", ((t_philo*)philo)->num + 1, lifetime(&((t_philo*)philo)->start_time, &((t_philo*)philo)->real_time));
//	pthread_mutex_unlock(&((t_philo*)philo)->info->block);

	if (!((t_philo*)philo)->info->died)
	{
		status = philo_eat(philo, ((t_philo*)philo)->left_fork, ((t_philo*)philo)->right_fork);
		if (!status && ((t_philo*)philo)->info->died == 1)
			return (NULL);
	}

	if (!((t_philo*)philo)->info->died)
	{
		status = philo_sleep(philo);
		if (!status && ((t_philo*)philo)->info->died == 1)
			return (NULL);
	}

	if (!((t_philo*)philo)->info->died)
	{
		status = philo_think(philo);
		if (!status && ((t_philo*)philo)->info->died == 1)
			return (NULL);
	}

//	pthread_mutex_lock(&((t_philo*)philo)->info->block);
	printf("%d - END1 %ld\n", ((t_philo*)philo)->num + 1, lifetime(&((t_philo*)philo)->start_time, &((t_philo*)philo)->real_time, 0));
//	pthread_mutex_unlock(&((t_philo*)philo)->info->block);

	return (philo);
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

t_info					*init_info(t_info *info, char **argv)
{
	pthread_mutex_init(&info->block, NULL);
	if (!params_for_philo(argv[1], &info->numb_of_philo))
		return (NULL);
	if (!params_for_philo(argv[2], &info->time_to_die))
		return (NULL);
	if (!params_for_philo(argv[3], &info->time_to_eat))
		return (NULL);
	if (!params_for_philo(argv[4], &info->time_to_sleep))
		return (NULL);
	if (!info->must_eat)
		if (!params_for_philo(argv[5], &info->numb_must_eat))
			return (NULL);

	if (info->numb_of_philo < 0 || info->time_to_die < 0 || info->time_to_eat < 0
	|| info->time_to_sleep < 0 || (!info->must_eat && info->numb_must_eat < 0))
	{
		write(2, INCORRECT_PARAMS, 26);
		return (NULL);
	}
	info->died = 0;
	lifetime(&info->start_time, &info->real_time, 1);
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
		philo[i].left_fork = (i + 1) % philo[i].info->numb_of_philo;
		philo[i].right_fork = i % philo[i].info->numb_of_philo;
		philo[i].num = i;
		philo[i].ate = 0;
		philo[i].died = 0;
		lifetime(&philo[i].start_time, &philo[i].real_time, 1);
		i++;
	}
}

int 					join_philo(t_philo *philo, int numb)
{
	int 				i;
	int 				status;
	void 				*ptr;

	i = 0;
	while (i < numb)
	{
		status = pthread_join(philo[i].philo, &ptr);
		if (status)
		{
//			printf("\e[1;31mphilo - %d\e[0m\n", philo[i].num + 1);
//			printf("\e[1;31mError: pthread_join\e[0m\n");
			write(2, "Error: pthread_join\n", 20);
			return (0);
		}
		if (!ptr && philo[i].died == 1)
		{
			print_message(&philo[i], lifetime(&philo[i].info->start_time, &philo[i].info->real_time, 0), PHILO_DIED, NULL);
			return (0);
		}
		i++;
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
			create_philo(philo, info.numb_of_philo);
			join_philo(philo, info.numb_of_philo);
			if (info.must_eat == info.numb_must_eat)
				break ;
			if (philo->ate == philo->info->numb_must_eat
				&& philo->info->must_eat != -1)
				break ;
			if (info.died)
				break ;
		}
		free(philo);
		destroy_init(&info);
	}
	else
		write(2, COUNT_PARAMS, 26);
	return (0);
}