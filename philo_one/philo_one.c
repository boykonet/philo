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

#define PHILO_TAKEN_FORK	"has taken a "					/* 12 */
#define TAKEN_LEFT_FORK		"left fork\n"					/* 10 */
#define TAKEN_RIGHT_FORK	"right fork\n"					/* 11 */
#define PHILO_EAT			"is eating\n"					/* 10 */
#define PHILO_SLEEP			"is sleeping\n"					/* 12 */
#define PHILP_THINK			"is thinking\n"					/* 12 */
#define PHILO_DIED			"died\n"						/* 5  */
#define COUNT_PARAMS		"Count params is incorrect\n"	/* 26 */
#define CORRECT_PARAMS		"Input params is incorrect\n"	/* 26 */

#define FALSE				0
#define TRUE				1

#include <pthread.h>		/*
							** pthread_create
							** pthread_detach
							** pthread_join
							** pthread_mutex_init
							** pthread_mutex_destroy
							** pthread_mutex_lock
							** pthread_mutex_unlock 
							*/
#include <unistd.h>			/*
							** write
							** usleep
							*/
#include <stdio.h>			/*
							** printf
							*/
#include <string.h>			/*
							** memset
							*/
#include <stdlib.h>			/*
							** malloc
							** free

							*/
#include <sys/time.h>		/*
							** gettimeofday
							*/


typedef struct			s_philo
{
	pthread_t			philo;
	pthread_mutex_t		left_fork;
	pthread_mutex_t		right_fork;
	int					num;
}						t_philo;

typedef struct			s_info
{
	int					numb;
	int					die;
	int					eat;
	int					sleep;
	int					numb_must_eat;
	int					must_eat;
	double				time;
	t_philo				**philo;
}						t_info;

int						ternar_int(int condition, int p1, int p2)
{
	if (condition)
		return p1;
	else
		return p2;
}

void					*routine(void *info)
{
	int					i;

	i = 0;
	while (i < (int)info->numb)
	{
		philo_eat(&(t_philo*)info->philo[i], (int)info->eat);
		philo_sleep((&(t_philo*)info->philo[i], (int)info->sleep);
		philo_think((&(t_philo*)info->philo[i], (int)info->think);
		i++;
	}
}

void					*philo_eat(void **philo, int time_to_eat)
{
	int					i = 5;

	pthread_mutex_lock(&(pthread_mutex_t*)philo->left_fork);
	pthread_mutex_lock(&(pthread_mutex_t*)philo->right_fork);
	write(1, PHILO_EAT, 9);
	write (i < 5)
		printf("%d\n ", arr[i++]);
	usleep(3000000);
	pthread_mutex_unlock(&(pthread_mutex_t*)philo->right_fork);
	pthread_mutex_unlock(&(pthread_mutex_t*)philo->left_fork);
	return (NULL);
}

void					*philo_sleep(t_philo **philo, int time_to_sleep)
{
}

void					*philo_think(t_philo **philo, int time_to_think)
{

}


t_info					*init_info(t_info *info, char **argv)
{
	info->numb = ft_atoi(argv[1]);
	info->die = ft_atoi(argv[2]);
	info->eat = ft_atoi(argv[3]);
	info->sleep = ft_atoi(argv[4]);
	info->numb_must_eat = ternar_int((argv[5] && !info->must_eat), \
	ft_atoi(argv[5]), 0);
	info->time = 0;
	if (info->numb < 0 || info->die < 0 || info->eat < 0
	|| info->sleep < 0 || info->numb_must_eat < 0)
	{
		write(2, CORRECT_PARAMS, 26);
		return (NULL);
	}
	return (info);

}

void					init_philo(t_philo **philo, int numb_of_philo)
{
	int		i;

	i = 0;
	while (i < numb_of_philo)
	{
		(*philo[i]).philo = NULL;
		pthread_mutex_init(&(*philo)[i].left_fork);
		pthread_mutex_init(&(*philo)[i].right_fork);
		(*philo)[i].num = i + 1;
		i++;
	}
}

void					destroy_philo(t_philo **philo, int numb_of_philo)
{
	int					i;

	i = 0;
	while (i < numb_of_philo)
	{
		pthread_join((*philo)[i++]->philo);
		(*philo)[i++]->philo = NULL;
	}
}

void					destroy_init(t_info *info)
{
	int					i;

	i = 0;
	free(info->philo);
}

int						main(int argc, char **argv)
{
	int					i;
	t_info				info;
	int					num;

	if (argc == 4 || argc == 5)
	{
		info.must_eat = ternar_int((argc == 5), 0, -1);
		if (!init_info(&info))
			continue ;
		init.philo = malloc(sizeof(t_philo*) * init.numb);
		if (!init.philo)
			continue ;
		while (TRUE)
		{
			i = 0;
			init_philo(&philo);
			while (i < init.numb_of_philo)
			{
				pthread_create(&thread1, NULL, &philo_eat, &(void*)philo[i]);

				destroy_philo(&philo);
				if (init.must_eat == init.numb_philo_must_eat)
					break ;
				destroy_philo(&init->philo[i]);
				i++;
			}
			/* usleep(3000000); */
		}
		destroy_init(&init);
	}
	else
		write(2, COUNT_PARAMS, 26);
	return (0);
}
