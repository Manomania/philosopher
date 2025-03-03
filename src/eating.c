/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:02:53 by maximart          #+#    #+#             */
/*   Updated: 2025/03/03 12:03:06 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_first_fork(t_philo *philo, int fork_hand)
{
	handle_fork_mutex(philo, fork_hand, LOCK);
	if (get_status(philo->data) != ALIVE)
	{
		handle_fork_mutex(philo, fork_hand, UNLOCK);
		return (1);
	}
	return (0);
}

static int	take_forks(t_philo *philo)
{
	int	first;
	int	second;

	if (philo->forks[RIGHT] < philo->forks[LEFT])
	{
		first = RIGHT;
		second = LEFT;
	}
	else
	{
		first = LEFT;
		second = RIGHT;
	}
	if (take_first_fork(philo, first))
		return (1);
	handle_fork_mutex(philo, second, LOCK);
	if (get_status(philo->data) != ALIVE)
	{
		handle_fork_mutex(philo, first, UNLOCK);
		handle_fork_mutex(philo, second, UNLOCK);
		return (1);
	}
	return (0);
}

static void	update_meal_status(t_philo *philo)
{
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_time = ft_time();
	philo->meals_eaten++;
	if (philo->data->must_eaten > 0
		&& philo->meals_eaten >= philo->data->must_eaten)
	{
		handle_mutex(&philo->data->full_lock, LOCK);
		philo->data->full++;
		handle_mutex(&philo->data->full_lock, UNLOCK);
	}
	handle_mutex(&philo->meal_lock, UNLOCK);
	philo_time(philo->data->tt_eat);
}

static void	release_forks(t_philo *philo)
{
	if (philo->forks[RIGHT] < philo->forks[LEFT])
	{
		handle_fork_mutex(philo, LEFT, UNLOCK);
		handle_fork_mutex(philo, RIGHT, UNLOCK);
	}
	else
	{
		handle_fork_mutex(philo, RIGHT, UNLOCK);
		handle_fork_mutex(philo, LEFT, UNLOCK);
	}
}

int	philo_eat(t_philo *philo)
{
	if (philo->data->nb_philo == 1)
	{
		philo_alone(philo);
		return (1);
	}
	if (take_forks(philo))
		return (1);
	philo_print(MSG_EAT, philo);
	update_meal_status(philo);
	release_forks(philo);
	return (1);
}
