/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maximart <maximart@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 08:35:15 by maximart          #+#    #+#             */
/*   Updated: 2025/02/10 08:35:18 by maximart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int argc, char **argv)
{
	long	value;
	int		i;
	int		j;

	if (ft_strlen(*argv) == 0 || ft_strlen(*argv) > 10)
		return (0);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf(RED"Error:\nYour values must be numeric only\n"RESET);
				return (1);
			}
			j++;
		}
		value = ft_atol(argv[i]);
		if (value > INT_MAX || value < 0)
		{
			printf(RED"Error:\nYour value is higher than a INT\n"RESET);
			return (1);
		}
		i++;
	}
	return (0);
}

t_data	*init_struct(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->nb_philo = ft_atoi(argv[1]);
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eaten = ft_atoi(argv[5]);
	else
		data->must_eaten = -1;
	if (data->nb_philo <= 0 || data->tt_die <= 0 || data->tt_eat <= 0 || data->tt_sleep <= 0)
	{
		free(data);
		return (NULL);
	}
	data->status = ALIVE;
	data->full = 0;
	return(data);
}

void	DEBUG_DATA(t_data *data)
{
	printf(YELLOW"DEBUG: time start %ld\n"RESET, data->time_start);
	printf(YELLOW"DEBUG: nb_philo %d\n"RESET, data->nb_philo);
	printf(YELLOW"DEBUG: tt_die %d\n"RESET, data->tt_die);
	printf(YELLOW"DEBUG: tt_eat %d\n"RESET, data->tt_eat);
	printf(YELLOW"DEBUG: tt_sleep %d\n"RESET, data->tt_sleep);
	printf(YELLOW"DEBUG: must_eaten %d\n"RESET, data->must_eaten);
	printf(YELLOW"DEBUG: dead %d\n"RESET, data->status);
}

void	DEBUG_PHILO(t_data *data)
{
	printf(YELLOW"\nDEBUG: last_meal_time %ld\n"RESET, data->philo->last_meal_time);
	printf(YELLOW"DEBUG: meals_eaten %d\n"RESET, data->philo->meals_eaten);
	for (int i = 0; i < data->nb_philo; i++)
		printf(YELLOW"DEBUG: ID %p\n"RESET, &data->philo[i].thread);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (check_args(argc, argv))
		return (1);
	if (!(argc == 5 || argc == 6))
	{
		printf(RED"Error:\nUsage : ./philo [number_of_philosophers] [time_to_die]"
			" [time_to_eat] [time_to_sleep]"
			" [# number_of_times_each_philosopher_must_eat]\n"RESET);
		return (1);
	}
	data = init_struct(argc, argv);
	if (!data)
	{
		printf(RED"Error:\nWrong arguments\n"RESET);
		return (1);
	}
	if (init_values(data))
	{
		free_ressources(data);
		return (1);
	}
	if (init_pthread_philo(data))
	{
		free_ressources(data);
		return (1);
	}
	free_ressources(data);
	return (0);
}