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



void free_ressources(t_data *data)
{
	int i;

	if (!data)
		return;

	/* Destroy all fork mutexes */
	if (data->forks_lock)
	{
		for (i = 0; i < data->nb_philo; i++)
			pthread_mutex_destroy(&data->forks_lock[i]);
		free(data->forks_lock);
	}

	/* Destroy all meal mutexes */
	if (data->philo)
	{
		for (i = 0; i < data->nb_philo; i++)
			pthread_mutex_destroy(&data->philo[i].meal_lock);
		free(data->philo);
	}

	/* Destroy the remaining mutexes */
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->full_lock);

	free(data);
}

void philo_print(char *msg, t_philo *philo)
{
	unsigned long current_time;
	int status;

	/* Get status first without holding the print lock */
	status = get_status(philo->data);

	/* Only proceed if philosophers are alive and not all full */
	if (status == ALIVE)
	{
		handle_mutex(&philo->data->print_lock, LOCK);

		/* Check status again after acquiring the lock to ensure consistency */
		status = get_status(philo->data);
		if (status == ALIVE)
		{
			current_time = ft_time() - philo->data->time_start;
			printf("%ld %d %s\n", current_time, philo->id + 1, msg);
		}

		handle_mutex(&philo->data->print_lock, UNLOCK);
	}
}

void philo_time(int duration)
{
	unsigned long start_time;
	unsigned long current_time;

	start_time = ft_time();
	while (1)
	{
		current_time = ft_time();
		if (current_time - start_time >= (unsigned long)duration)
			break;
		usleep(100);
	}
}
int philo_eat(t_philo *philo)
{
	if (philo->data->nb_philo == 1)
	{
		handle_fork_mutex(philo, RIGHT, LOCK);
		philo_print(MSG_FORK, philo);
		philo_time(philo->data->tt_die);
		handle_fork_mutex(philo, RIGHT, UNLOCK);
		return 0;
	}
	if (philo->id % 2 == 0)
	{
		handle_fork_mutex(philo, RIGHT, LOCK);
		if (get_status(philo->data) != ALIVE)
		{
			handle_fork_mutex(philo, RIGHT, UNLOCK);
			return 0;
		}
		handle_fork_mutex(philo, LEFT, LOCK);
		if (get_status(philo->data) != ALIVE)
		{
			handle_fork_mutex(philo, RIGHT, UNLOCK);
			handle_fork_mutex(philo, LEFT, UNLOCK);
			return 0;
		}
	}
	else
	{
		handle_fork_mutex(philo, LEFT, LOCK);
		if (get_status(philo->data) != ALIVE)
		{
			handle_fork_mutex(philo, LEFT, UNLOCK);
			return 0;
		}
		handle_fork_mutex(philo, RIGHT, LOCK);
		if (get_status(philo->data) != ALIVE)
		{
			handle_fork_mutex(philo, LEFT, UNLOCK);
			handle_fork_mutex(philo, RIGHT, UNLOCK);
			return 0;
		}
	}
	philo_print(MSG_EAT, philo);
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_time = ft_time();
	philo->meals_eaten++;
	if (philo->data->must_eaten > 0 && philo->meals_eaten >= philo->data->must_eaten)
	{
		handle_mutex(&philo->data->full_lock, LOCK);
		philo->data->full++;
		handle_mutex(&philo->data->full_lock, UNLOCK);
	}
	handle_mutex(&philo->meal_lock, UNLOCK);
	philo_time(philo->data->tt_eat);
	if (philo->id % 2 == 0)
	{
		handle_fork_mutex(philo, LEFT, UNLOCK);
		handle_fork_mutex(philo, RIGHT, UNLOCK);
	}
	else
	{
		handle_fork_mutex(philo, RIGHT, UNLOCK);
		handle_fork_mutex(philo, LEFT, UNLOCK);
	}
	return 1;
}

void *routine(void *arg)
{
	t_philo *philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;

	/* Stagger philosopher start times to prevent deadlock */
	if (philo->id % 2 && data->nb_philo > 1)
		usleep(1000); /* Small delay to break potential symmetry */

	/* Set the initial last meal time */
	handle_mutex(&philo->meal_lock, LOCK);
	philo->last_meal_time = ft_time();
	handle_mutex(&philo->meal_lock, UNLOCK);

	/* Main philosopher cycle */
	while (get_status(data) == ALIVE)
	{
		/* Check if we've reached the required number of meals */
		handle_mutex(&philo->meal_lock, LOCK);
		if (data->must_eaten > 0 && philo->meals_eaten >= data->must_eaten)
		{
			handle_mutex(&philo->meal_lock, UNLOCK);
			break;
		}
		handle_mutex(&philo->meal_lock, UNLOCK);

		/* Try to eat */
		if (philo_eat(philo) && get_status(data) == ALIVE)
		{
			/* Sleep phase */
			philo_print(MSG_SLEEP, philo);
			philo_time(data->tt_sleep);

			/* Think phase */
			if (get_status(data) == ALIVE)
				philo_print(MSG_THINK, philo);
		}
	}

	return (NULL);
}
