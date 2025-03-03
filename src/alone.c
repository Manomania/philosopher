/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alone.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:08:14 by maximart          #+#    #+#             */
/*   Updated: 2025/03/03 16:08:17 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_alone(t_philo *philo)
{
	handle_fork_mutex(philo, RIGHT, LOCK);
	philo_print(MSG_FORK, philo);
	philo_time(philo->data->tt_die);
	handle_fork_mutex(philo, RIGHT, UNLOCK);
}
