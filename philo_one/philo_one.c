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

int						gettime_print(t_philo *philo, char *p1, char *p2)
{
	if (gettimeofday(&philo->info->real_time, NULL) == -1)
		return (0);
	if (philo->info->start_time.tv_sec == 0 && philo->info->start_time.tv_usec == 0)
	{
		philo->info->start_time.tv_sec = philo->info->real_time.tv_sec;
		philo->info->start_time.tv_usec = philo->info->real_time.tv_usec;
	}
	printf("%ld %d %s", (philo->info->real_time.tv_sec - philo->info->start_time.tv_sec) * 1000
	+ (philo->info->real_time.tv_usec - philo->info->start_time.tv_usec) / 1000, philo->num, p1);
	if (p2)
		printf(" %s", p2);
	printf("\n");
	return (1);
}

int						philo_eat(t_philo *philo, int left_fork, int right_fork)
{
	pthread_mutex_lock(&philo->info->forks[left_fork]);
	gettime_print(philo, TAKEN_FORK, LEFT_FORK);

	pthread_mutex_lock(&philo->info->forks[right_fork]);
	gettime_print(philo, TAKEN_FORK, RIGHT_FORK);

	pthread_mutex_lock(&philo->info->block_eat);

	gettime_print(philo, PHILO_EAT, NULL);
	myusleep(philo->info->eat * 1000);
	philo->life_cycle += philo->info->eat;
	if (philo->life_cycle >= philo->info->die)
		return (0);

	pthread_mutex_unlock(&philo->info->block_eat);

	pthread_mutex_unlock(&philo->info->forks[right_fork]);
	gettime_print(philo, PUT_FORK, RIGHT_FORK);

	pthread_mutex_unlock(&philo->info->forks[left_fork]);
	gettime_print(philo, PUT_FORK, LEFT_FORK);
	return (1);
}

int						philo_sleep(t_philo *philo)
{
	return (1);
}

int						philo_think(t_philo *philo)
{
	return (1);
}

void					*routine(void *philo)
{
	int					i;
	int 				status;

	i = 0;
	while (i < ((t_philo*)philo)->info->numb)
	{
		status = philo_eat(&((t_philo*)philo)[i], ((t_philo*)philo)[i].left_fork, ((t_philo*)philo)[i].right_fork);
		if (!status || ((t_philo*)philo)[i].life_cycle >= ((t_philo*)philo)[i].info->die)
			return (NULL);
		status = philo_sleep(&((t_philo*)philo)[i]);

		status = philo_think(&((t_philo*)philo)[i]);
		i += 2;
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

t_info					*init_info(t_info *info, char **argv)
{
	info->numb = ft_atoi(argv[1]);
	info->die = ft_atoi(argv[2]);
	info->eat = ft_atoi(argv[3]);
	info->sleep = ft_atoi(argv[4]);
	info->numb_must_eat = ternar_int((argv[5] && !info->must_eat), \
	ft_atoi(argv[5]), 0);

	if (info->numb < 0 || info->die < 0 || info->eat < 0
	|| info->sleep < 0 || info->numb_must_eat < 0)
	{
		write(2, CORRECT_PARAMS, 26);
		return (NULL);
	}
	info->start_time.tv_sec = 0;
	info->start_time.tv_usec = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->numb);
	if (!info->forks)
		return (NULL);
	init_forks(info->forks);
	return (info);

}

void					init_philos(t_philo *philo, t_info *info)
{
	int		i;

	i = 0;
	while (i < info->numb)
	{
		philo[i].info = info;
		philo[i].life_cycle = 0;
		philo[i].left_fork = (i + 1) % philo[i].info->numb;
		philo[i].right_fork = i % philo[i].info->numb;
		philo[i].num = i + 1;
		i++;
	}
}

int 					create_and_destroy_philo(t_philo *philo, int numb)
{
	int 				start;
	int 				end;
	int 				status;

	start = 0;
	end = 0;
	while (start < numb)
	{
		status = pthread_create(&philo[start++].philo, NULL, routine, (void*)philo);
		if (status)
		{
			write(2, "Error: pthread_create\n", 22);
			return (0);
		}
	}
	while (end < numb)
	{
		status = pthread_join(philo[end++].philo, NULL);
		if (status)
		{
			write(2, "Error: pthread_join\n", 20);
			return (0);
		}
	}
	return (1);
}

void					destroy_philo(t_philo *philo)
{
	int					i;

	i = 0;
	while (i < philo->info->numb)
	{
		pthread_join(philo[i].philo, NULL);
	}
}

void					destroy_init(t_info *info)
{
	int					i;

	i = 0;
}

int						main(int argc, char **argv)
{
	int i;
	t_info				info;
	t_philo				*philo;

	int					num;
	if (argc == 5 || argc == 6)
	{
		info.must_eat = ternar_int((argc == 6), 0, -1);
		if (!init_info(&info, argv))
			return (-1);
		philo = malloc(sizeof(t_philo) * info.numb);
		if (!philo)
			return (0);
		init_philos(philo, &info);
		myusleep(200000);
		while (TRUE)
		{
			create_and_destroy_philo(philo, info.numb);
			if (info.must_eat == info.numb_must_eat)
				break ;
		}
		free(philo);
		destroy_init(&info);
	}
	else
		write(2, COUNT_PARAMS, 26);
	return (0);
}