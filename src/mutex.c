/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:39:30 by maximart          #+#    #+#             */
/*   Updated: 2025/02/10 12:39:34 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_mutex(pthread_mutex_t *mutex, t_padlock action)
{
	if (action == LOCK)
	{
		if (pthread_mutex_lock(mutex))
			return (1);
	}
	else if (action == UNLOCK)
	{
		if (pthread_mutex_unlock(mutex))
			return (1);
	}
	return (0);
}

int	handle_fork_mutex(t_philo *philo, t_hand hand, t_padlock action)
{
	if (action == LOCK)
	{
		if (pthread_mutex_lock(&philo->data->forks_lock[philo->forks[hand]]))
			return (1);
		if (get_status(philo->data) == ALIVE)
			philo_print(MSG_FORK, philo);
	}
	if (action == UNLOCK)
	{
		if (pthread_mutex_unlock(&philo->data->forks_lock[philo->forks[hand]]))
			return (1);
	}
	return (0);
}
