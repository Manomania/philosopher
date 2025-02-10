/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 08:35:33 by maximart          #+#    #+#             */
/*   Updated: 2025/02/10 08:35:35 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_timeval
{
	time_t		tv_sec;
	suseconds_t	tv_usec;
}				t_timeval;

typedef struct	s_philo
{
	int	id;
	int	l_fork;
	int	r_fork;
	int	start_eat;
}		t_philo;

typedef struct s_data
{
	long	time_start;
	int		nb_philo;
	int		tt_die;
	int		tt_eat;
	int		tt_sleep;
	int		must_eaten;
	int		finished;
}		t_data;



#endif
