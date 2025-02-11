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
				return (0);
			j++;
		}
		value = ft_atol(argv[i]);
		if (value > INT_MAX || value < 0)
			return (0);
		i++;
	}
	return (1);
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
	data->dead = 0;
	return(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!check_args(argc, argv))
		return (1);
	data = init_struct(argc, argv);
	if (!data)
		return (1);
	free(data);
	return (0);
}