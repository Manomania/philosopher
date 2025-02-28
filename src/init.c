/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:40:58 by maximart          #+#    #+#             */
/*   Updated: 2025/02/10 12:41:00 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i;
		data->philo[i].forks[0] = data->philo[i].id;
		data->philo[i].forks[1] = (data->philo[i].id + 1) % data->nb_philo;
		data->philo[i].meals_eaten = 0;
		data->philo[i].last_meal_time = 0;
		data->philo[i].data = data;
		data->philo[i].available = 0;
		i++;
	}
	if (i != data->nb_philo)
		return (1);
	return (0);
}

static int init_mutex(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&(data->forks_lock[i]), NULL))
			return (1);
		i++;
	}
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&(data->philo[i].meal_lock), NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->death_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->full_lock, NULL))
		return (1);

	return (0);
}

int	init_values(t_data *data)
{
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (1);
	data->forks_lock = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks_lock)
		return (1);
	if (init_mutex(data))
	{
		free(data->philo);
		free(data->forks_lock);
		return (1);
	}
	if (init_philo(data))
	{
		free(data->philo);
		free(data->forks_lock);
		return (1);
	}
	return (0);
}

void *monitoring(void *arg)
{
	t_data *data;
	int i;
	unsigned long current_time;
	int is_dead;

	data = (t_data *)arg;

	while (1)
	{
		/* Check if all philosophers are full */
		handle_mutex(&data->full_lock, LOCK);
		if (data->full >= data->nb_philo)
		{
			handle_mutex(&data->death_lock, LOCK);
			data->status = FULL;
			handle_mutex(&data->death_lock, UNLOCK);
			handle_mutex(&data->full_lock, UNLOCK);
			break;
		}
		handle_mutex(&data->full_lock, UNLOCK);

		/* Check if any philosopher has died */
		i = 0;
		is_dead = 0;
		while (i < data->nb_philo && !is_dead)
		{
			current_time = ft_time();

			handle_mutex(&data->philo[i].meal_lock, LOCK);
			if ((current_time - data->philo[i].last_meal_time) > (unsigned long)data->tt_die)
			{
				handle_mutex(&data->death_lock, LOCK);
				data->status = DEAD;
				handle_mutex(&data->death_lock, UNLOCK);

				/* Print death message */
				handle_mutex(&data->print_lock, LOCK);
				printf("%lu %d %s\n", current_time - data->time_start, i + 1, MSG_DIE);
				handle_mutex(&data->print_lock, UNLOCK);

				is_dead = 1;
			}
			handle_mutex(&data->philo[i].meal_lock, UNLOCK);
			i++;
		}

		if (is_dead || get_status(data) != ALIVE)
			break;

		/* Sleep to avoid consuming too much CPU */
		usleep(1000);
	}

	return NULL;
}

int init_pthread_philo(t_data *data)
{
	int i;
	pthread_t monitor;

	/* Set start time */
	data->time_start = ft_time();

	/* Initialize all philosophers' last meal time */
	for (i = 0; i < data->nb_philo; i++)
	{
		handle_mutex(&data->philo[i].meal_lock, LOCK);
		data->philo[i].last_meal_time = data->time_start;
		handle_mutex(&data->philo[i].meal_lock, UNLOCK);
	}

	/* Create philosopher threads */
	for (i = 0; i < data->nb_philo; i++)
	{
		if (pthread_create(&data->philo[i].thread, NULL, routine, &data->philo[i]))
			return (0);
	}

	/* Create monitoring thread */
	if (pthread_create(&monitor, NULL, monitoring, data))
		return (0);

	/* Join philosopher threads */
	for (i = 0; i < data->nb_philo; i++)
	{
		if (pthread_join(data->philo[i].thread, NULL))
			return (0);
	}

	/* Join monitoring thread */
	if (pthread_join(monitor, NULL))
		return (0);

	return (1);
}
