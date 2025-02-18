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
	unsigned long	current_time;

	pthread_mutex_lock(&philo->data->print_lock);
	current_time = ft_time() - philo->data->time_start;
	if (!philo->data->dead)
		printf("%ld %d %s\n", current_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
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
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = ft_time();
	pthread_mutex_unlock(&philo->meal_lock);

	// philo_sleep(duration, philo->data); il doit "dormir" pour simuler un repas pendant un certain temps

	pthread_mutex_unlock(&philo->data->forks_lock[philo->forks[0]]);
	if (philo->data->nb_philo > 1)
		pthread_mutex_unlock(&philo->data->forks_lock[philo->forks[1]]);
}

void	philo_sleep(int duration, t_data *data)
{

}

void	*routine(void *arg)
{
	t_philo *philo;
	t_data	*data;
	int		i;

	i = 0;
	philo = (t_philo *)arg;
	data = philo->data;
	while (!data->dead || philo->meals_eaten != data->must_eaten)
	{
		philo_eat(philo);
		philo_sleep(data->tt_sleep, data);
	}
	return (NULL);
}
