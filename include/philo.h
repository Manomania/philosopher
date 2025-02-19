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

/*******************************************************************************
*                                    Macros                                    *
*******************************************************************************/

# define RESET		"\033[039m"
# define RED		"\033[091m"
# define GREEN		"\033[092m"
# define YELLOW		"\033[093m"

# define MSG_FORK	"has taken a fork"
# define MSG_EAT	"is eating"
# define MSG_SLEEP	"is sleeping"
# define MSG_THINK	"is thinking"
# define MSG_DIE	"died"

/*******************************************************************************
*                                  Structures                                  *
*******************************************************************************/

typedef struct s_philo	t_philo;
typedef struct s_data	t_data;
typedef struct timeval	t_timeval;

typedef struct s_data
{
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philo;
	unsigned long	time_start;
	int				nb_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eaten;
	int				full;
	int				dead;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	struct s_data	*data;
	unsigned long	last_meal_time;
	int				id;
	int				meals_eaten;
	int				forks[2];
}					t_philo;

enum e_padlock
{
	LOCK = 1,
	UNLOCK = 0,
};

/*******************************************************************************
*                             Function Prototypes                              *
*******************************************************************************/

// utils.c
int				ft_atoi(const char *str);
long			ft_atol(const char *str);
size_t			ft_strlen(char *str);
unsigned long	ft_time(void);

// init.c
int	init_values(t_data *data);
int	init_pthread_philo(t_data *data);

// threads.c
void	*routine(void *arg);

#endif
