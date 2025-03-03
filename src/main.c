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

static int	check_value(int argc, char **argv)
{
	long	value;
	int		i;

	i = 0;
	while (++i < argc)
	{
		value = ft_atol(argv[i]);
		if (value > INT_MAX || value < 0)
		{
			printf(RED"Error:\nYour value is higher than "
				"a INT or under 0\n"RESET);
			return (1);
		}
	}
	return (0);
}

static int	check_args(int argc, char **argv)
{
	int		i;
	int		j;

	if (ft_strlen(*argv) == 0 || ft_strlen(*argv) > 10)
		return (0);
	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf(RED"Error:\nYour values must be"
					"numerical and positive only\n"RESET);
				return (1);
			}
			j++;
		}
	}
	return (0);
}

static t_data	*init_struct(int argc, char **argv)
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
	if (data->nb_philo <= 0 || data->tt_die <= 0
		|| data->tt_eat <= 0 || data->tt_sleep <= 0)
	{
		free(data);
		return (NULL);
	}
	data->status = ALIVE;
	data->full = 0;
	return (data);
}

static int	error_init(t_data *data)
{
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

int	main(int argc, char **argv)
{
	t_data	*data;

	if (check_args(argc, argv) || check_value(argc, argv))
		return (1);
	if (!(argc == 5 || argc == 6))
	{
		printf(RED"Error:\nUsage : ./philo [number_of_philosophers] "
			"[time_to_die] [time_to_eat] [time_to_sleep]"
			" [# number_of_times_each_philosopher_must_eat]\n"RESET);
		return (1);
	}
	data = init_struct(argc, argv);
	if (!data)
	{
		printf(RED"Error:\nWrong arguments\n"RESET);
		return (1);
	}
	if (error_init(data))
		return (1);
	return (0);
}
