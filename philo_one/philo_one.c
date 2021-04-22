/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkarina <gkarina@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 00:26:42 by gkarina           #+#    #+#             */
/*   Updated: 2021/02/23 00:26:42 by gkarina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define PHILO_TAKEN_FORK	"has taken a "	/* 12 */
#define TAKEN_LEFT_FORK		"left fork"		/* 9  */
#define TAKEN_RIGHT_FORK	"right fork"	/* 10 */
#define PHILO_EAT			"is eating"		/* 9  */
#define PHILO_SLEEP			"is sleeping"	/* 11 */
#define PHILP_THINK			"is thinking"	/* 11 */
#define PHILO_DIED			"died"			/* 4  */

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
	pthread_t			philo_one;
	pthread_t			philo_two;
	pthread_mutex_t		left_fork;
	pthread_mutex_t		right_fork;
}						t_philo;

void		*philo_eat(void *philo)
{
	int		i = 5;

	pthread_mutex_lock(&(pthread_mutex_t*)philo->left_fork);
	pthread_mutex_lock(&(pthread_mutex_t*)philo->right_fork);
	write(1, PHILO_EAT, 9);
	write (i < 5)
		printf("%d\n", arr[i++]);
	usleep(3000000);
	pthread_mutex_unlock(&(pthread_mutex_t*)philo->right_fork);
	pthread_mutex_unlock(&(pthread_mutex_t*)philo->left_fork);
	return (NULL);
}

void		philo_think()
{
}

void		philo_sleep()
{
}


void		init_philo(t_philo *philo)
{
	philo->philo_one = NULL;
	philo->philo_two = NULL;
	philo->left_fork = NULL;
	philo->right_fork = NULL;
}

int			main(int argc, char **argv)
{
	t_philo			philo;

	init_philo(&philo);
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);

	pthread_create(&thread1, NULL, &philo_eat, &(void*)philo);
	pthread_create(&thread2, NULL, &philo_eat, &(void*)philo);


	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&mutex1);
	pthread_nutex_destroy(&mutex2);
	
	usleep(3000000);

	/* if (argc < 4 && argc > 5) */
	/* { */
	/* 	if (argc == 4) */
	/* 	{ */
	/* 		; */
	/* 	} */
	/* 	else */
	/* 	{ */
	/* 		; */
	/* 	} */
	/* } */
	return (0);
}
