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

typedef enum e_padlock
{
	LOCK,
	UNLOCK,
}	e_padlock;

typedef enum e_status
{
	ALIVE,
	DEAD,
	FULL,
}	e_status;

typedef enum e_hand
{
	RIGHT,
	LEFT,
}	e_hand;

typedef struct s_data
{
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	full_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	e_status		status;
	t_philo			*philo;
	unsigned long	time_start;
	int				nb_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eaten;
	int				full;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
	t_data			*data;
	unsigned long	last_meal_time;
	int				id;
	int				meals_eaten;
	int				forks[2];
}					t_philo;



/*******************************************************************************
*                             Function Prototypes                              *
*******************************************************************************/

// utils.c
int				ft_atoi(const char *str);
long			ft_atol(const char *str);
size_t			ft_strlen(char *str);
unsigned long	ft_time(void);

// init.c
int				init_values(t_data *data);
int				init_pthread_philo(t_data *data);

// threads.c
void			*routine(void *arg);
void			free_ressources(t_data *data);

// mutex.c
int				handle_mutex(pthread_mutex_t *mutex, e_padlock action);
int	handle_fork_mutex(t_philo *philo, e_hand hand, e_padlock action);
int	handle_forks(t_philo *philo, e_padlock action);

int	increase_meal(t_philo *philo);
int	increase_full(t_philo *philo);
void	philo_print(char *msg, t_philo *philo);
int	set_status(t_data *data);
unsigned long	set_last_meal(t_philo *philo);
int	get_status(t_data *data);

#endif
