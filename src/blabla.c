/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blabla.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:33:57 by maximart          #+#    #+#             */
/*   Updated: 2025/02/24 15:34:47 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	increase_full(t_philo *philo)
{
	handle_mutex(&philo->data->full_lock, LOCK);
	if (philo->meals_eaten == philo->data->must_eaten)
		philo->data->full++;
	handle_mutex(&philo->data->full_lock, UNLOCK);
	return (philo->data->full);
}

int	increase_meal(t_philo *philo)
{
	handle_mutex(&philo->meal_lock, LOCK);
	philo->meals_eaten++;
	handle_mutex(&philo->meal_lock, UNLOCK);
	return (philo->meals_eaten);
}

int get_status(t_data *data)
{
	int	status_value;

	handle_mutex(&data->death_lock, LOCK);
	status_value = data->status;
	handle_mutex(&data->death_lock, UNLOCK);
	return (status_value);
}

unsigned long	set_last_meal(t_philo *philo)
{
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_time = ft_time();
	handle_mutex(&philo->meal_lock, UNLOCK);
	return (philo->last_meal_time);
}

int set_status(t_data *data)
{
	int i;
	unsigned long current_time;

	/* Check if all philosophers are full */
	handle_mutex(&data->full_lock, LOCK);
	if (data->full == data->nb_philo)
	{
		data->status = FULL;
		handle_mutex(&data->full_lock, UNLOCK);
		return 1;
	}
	handle_mutex(&data->full_lock, UNLOCK);

	/* Check if any philosopher has died */
	i = 0;
	while (i < data->nb_philo)
	{
		current_time = ft_time();
		handle_mutex(&data->philo[i].meal_lock, LOCK);
		if ((current_time - data->philo[i].last_meal_time) > (unsigned long)data->tt_die)
		{
			handle_mutex(&data->death_lock, LOCK);
			data->status = DEAD;
			handle_mutex(&data->death_lock, UNLOCK);
			handle_mutex(&data->philo[i].meal_lock, UNLOCK);
			philo_print(MSG_DIE, &data->philo[i]);
			return 1;
		}
		handle_mutex(&data->philo[i].meal_lock, UNLOCK);
		i++;
	}

	return 0;
}
