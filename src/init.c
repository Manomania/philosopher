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
		data->philo[i].id = i + 1;
		data->philo[i].lfork = data->philo[i].id;
		data->philo[i].rfork = (data->philo[i].id + 1) % data->nb_philo;
		data->philo[i].meals_eaten = 0;
		data->philo[i].last_meal_time = 0;
		data->philo[i].data = data;
		i++;
	}
	if (i != data->nb_philo)
		return (1);
	return (0);
}

static int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&(data->forks_lock[i]), NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->philo->meal_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->death_lock, NULL))
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

