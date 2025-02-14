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

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define RESET	"\033[039m"
# define RED	"\033[091m"
# define GREEN	"\033[092m"
# define YELLOW	"\033[093m"

typedef struct s_philo	t_philo;
typedef struct s_data	t_data;
typedef struct timeval	t_timeval;

typedef struct s_data
{
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philo;
	long			time_start;
	int				nb_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eaten;
	int				dead;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
	struct s_data	*data;
	long			last_meal_time;
	int				id;
	int				lfork;
	int				rfork;
	int				meals_eaten;
}					t_philo;

// utils.c
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
size_t	ft_strlen(char *str);

// init.c
int	init_values(t_data *data);
#endif
