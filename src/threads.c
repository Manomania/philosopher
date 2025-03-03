/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:26:52 by maximart          #+#    #+#             */
/*   Updated: 2025/02/17 13:26:56 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_ressources(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks_lock)
	{
		i = -1;
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->forks_lock[i]);
		free(data->forks_lock);
	}
	if (data->philo)
	{
		i = -1;
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->philo[i].meal_lock);
		free(data->philo);
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->full_lock);
	free(data);
}

void	philo_print(char *msg, t_philo *philo)
{
	unsigned long	current_time;
	int				status;

	status = get_status(philo->data);
	if (status == ALIVE)
	{
		handle_mutex(&philo->data->print_lock, LOCK);
		status = get_status(philo->data);
		if (status == ALIVE)
		{
			current_time = ft_time() - philo->data->time_start;
			printf("%ld %d %s\n", current_time, philo->id + 1, msg);
		}
		handle_mutex(&philo->data->print_lock, UNLOCK);
	}
}

// void	philo_time(int duration)
// {
// 	unsigned long	start_time;
// 	unsigned long	current_time;
//
// 	start_time = ft_time();
// 	while (1)
// 	{
// 		current_time = ft_time();
// 		if ((current_time - start_time) >= (unsigned long)duration)
// 			break ;
// 		usleep(100);
// 	}
// }

void	philo_time(int duration)
{
	unsigned long	start_time;
	unsigned long	current_time;
	unsigned long	elapsed;

	start_time = ft_time();
	while (1)
	{
		current_time = ft_time();
		elapsed = current_time - start_time;
		if (elapsed >= (unsigned long)duration)
			break ;
		if (duration - elapsed > 10)
			usleep(1000);
		else
			usleep(100);
	}
}

static void	track_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	while (get_status(data) == ALIVE)
	{
		handle_mutex(&philo->meal_lock, LOCK);
		if (data->must_eaten > 0 && philo->meals_eaten >= data->must_eaten)
		{
			handle_mutex(&philo->meal_lock, UNLOCK);
			break ;
		}
		handle_mutex(&philo->meal_lock, UNLOCK);
		if (philo_eat(philo) && get_status(data) == ALIVE)
		{
			philo_print(MSG_SLEEP, philo);
			philo_time(data->tt_sleep);
			if (get_status(data) == ALIVE)
			{
				philo_print(MSG_THINK, philo);
				usleep(500);
			}
		}
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2)
		usleep(philo->data->tt_eat * 100);
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_time = ft_time();
	handle_mutex(&philo->meal_lock, UNLOCK);
	track_philo(philo);
	return (NULL);
}
