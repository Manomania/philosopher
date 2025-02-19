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

void	philo_print(char *msg, t_philo *philo)
{
	unsigned long current_time;

	if (!philo->data->dead && philo->data->full != philo->data->nb_philo)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		current_time = ft_time() - philo->data->time_start;
		printf("%ld %d %s\n", current_time, philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

void	philo_time(int duration, t_data *data)
{
	unsigned long	wake_up;

	wake_up = ft_time() + duration;
	while (ft_time() < wake_up && !data->dead)
		usleep(100);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks_lock[philo->forks[0]]);
	philo_print(MSG_FORK, philo);
	if (philo->data->nb_philo > 1)
	{
		pthread_mutex_lock(&philo->data->forks_lock[philo->forks[1]]);
		philo_print(MSG_FORK ,philo);
	}
	philo_print(MSG_EAT, philo);
	philo_time(philo->data->tt_eat, philo->data);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal_time = ft_time();
	philo->meals_eaten++;
	if (philo->meals_eaten == philo->data->must_eaten)
		philo->data->full++;
	pthread_mutex_unlock(&philo->data->meal_lock);
	pthread_mutex_unlock(&philo->data->forks_lock[philo->forks[0]]);
	if (philo->data->nb_philo > 1)
		pthread_mutex_unlock(&philo->data->forks_lock[philo->forks[1]]);
}

int	philo_dead(t_data *data)
{
	unsigned long	current_time;

	current_time = ft_time() - data->time_start;

	printf(YELLOW"DEBUG: current_time %ld\n"RESET, ft_time());
	printf(YELLOW"DEBUG: last_meal_time %ld\n"RESET, data->philo->last_meal_time);

	pthread_mutex_lock(&data->death_lock);
	if ((current_time - data->philo->last_meal_time) > (unsigned long)data->philo->data->tt_die)
	{
		philo_print(MSG_DIE, data->philo);
		data->dead = 1;
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}

void	*routine(void *arg)
{
	t_philo *philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;

	if (philo->id % 2 && data->nb_philo > 1)
	{
		philo_time(data->tt_eat / 50, data);
		philo_print(MSG_THINK, philo);
	}
	while (!data->dead && philo->meals_eaten < philo->data->must_eaten)
	{
		if (!philo_dead(philo->data))
			break;
		printf(GREEN"DEBUG: Data->dead %d\n"RESET, data->dead);
		philo_eat(philo);
		philo_print(MSG_SLEEP, philo);
		philo_time(data->tt_sleep, data);
		philo_print(MSG_THINK, philo);
	}
	if (philo->data->full == philo->data->nb_philo)
	{
		printf(GREEN"DEBUG: Philo are full\n"RESET);
		exit(1);
	}
	return (NULL);
}
