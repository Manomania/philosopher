/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:19:45 by maximart          #+#    #+#             */
/*   Updated: 2025/03/03 10:19:48 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_status(t_data *data)
{
	int	status_value;

	handle_mutex(&data->death_lock, LOCK);
	status_value = data->status;
	handle_mutex(&data->death_lock, UNLOCK);
	return (status_value);
}

static int	is_philosopher_dead(t_data *data, int i, unsigned long current_time)
{
	unsigned long	time_since_last_meal;
	int				is_dead;

	is_dead = 0;
	handle_mutex(&data->philo[i].meal_lock, LOCK);
	time_since_last_meal = current_time - data->philo[i].last_meal_time;
	if (time_since_last_meal >= (unsigned long)data->tt_die)
	{
		handle_mutex(&data->death_lock, LOCK);
		if (data->status == ALIVE)
		{
			data->status = DEAD;
			handle_mutex(&data->death_lock, UNLOCK);
			handle_mutex(&data->print_lock, LOCK);
			printf("%lu %d %s\n", (current_time - data->time_start),
				i + 1, MSG_DIE);
			handle_mutex(&data->print_lock, UNLOCK);
			is_dead = 1;
		}
		else
			handle_mutex(&data->death_lock, UNLOCK);
	}
	handle_mutex(&data->philo[i].meal_lock, UNLOCK);
	return (is_dead);
}

static int	check_if_all_full(t_data *data)
{
	int	all_full;

	all_full = 0;
	if (data->must_eaten <= 0)
		return (0);
	handle_mutex(&data->full_lock, LOCK);
	if (data->full >= data->nb_philo)
	{
		handle_mutex(&data->death_lock, LOCK);
		if (data->status == ALIVE)
			data->status = FULL;
		handle_mutex(&data->death_lock, UNLOCK);
		all_full = 1;
	}
	handle_mutex(&data->full_lock, UNLOCK);
	return (all_full);
}

void	*monitoring(void *arg)
{
	t_data			*data;
	unsigned long	current_time;
	int				i;

	data = (t_data *)arg;
	usleep(1000);
	while (get_status(data) == ALIVE)
	{
		if (check_if_all_full(data))
			break ;
		i = 0;
		while (i < data->nb_philo && get_status(data) == ALIVE)
		{
			current_time = ft_time();
			if (is_philosopher_dead(data, i, current_time))
				break ;
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
