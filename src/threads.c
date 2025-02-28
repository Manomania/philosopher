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

	i = 0;

	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->philo->meal_lock);
	pthread_mutex_destroy(&data->death_lock);

}

void	philo_print(char *msg, t_philo *philo)
{
	unsigned long	current_time;

	if (get_status(philo->data) && philo->data->full != philo->data->nb_philo)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		current_time = ft_time() - philo->data->time_start;
		printf("%ld %d %s\n", current_time, philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

void	philo_time(int duration)
{
	unsigned long	wake_up;

	wake_up = ft_time() + duration;
	while (ft_time() < wake_up)
	{
		usleep(100);
	}
}

void	philo_eat(t_philo *philo)
{
	handle_fork_mutex(philo, RIGHT, LOCK);
	if (philo->data->nb_philo > 1)
	{
		handle_fork_mutex(philo, LEFT, LOCK);
		philo_print(MSG_EAT, philo);
		increase_meal(philo);
		set_last_meal(philo);
		philo_time(philo->data->tt_eat);
		increase_full(philo);
		handle_fork_mutex(philo, RIGHT, UNLOCK);
	}
	handle_fork_mutex(philo, LEFT, UNLOCK);
}

void	*routine(void *arg)
{
	t_philo *philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;

	if (philo->id % 2 && data->nb_philo > 1)
		philo_time(data->tt_eat / 50);
	while (((philo->meals_eaten < philo->data->must_eaten || philo->data->must_eaten < 0)))
	{
		philo_eat(philo);
		philo_print(MSG_SLEEP, philo);
		philo_time(data->tt_sleep);
		philo_print(MSG_THINK, philo);

	}
	if (philo->data->full == philo->data->nb_philo)
	{
		printf(GREEN"DEBUG: Philo are full\n"RESET);
		exit(1);
	}
	return (NULL);
}
