/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:39:01 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:39:04 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arg_content(const char *arg)
{
	int	i;

	if (!arg || !*arg)
		return (1);
	i = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

/* tüm CLI doğrulaması: kısa hatalar, senin print_error kullanımı */
static int	check_valid_args(int ac, char **av)
{
	if (!(ac == 5 || ac == 6))
		return (print_error("Usage: ./philo n t_die t_eat t_sleep [max]\n"));
	if (check_arg_content(av[1]) || ft_atoi(av[1]) <= 0 || exceeds_int_max(av[1]))
		return (print_error("Invalid philosophers\n"));
	if (check_arg_content(av[2]) || ft_atoi(av[2]) <= 0 || exceeds_int_max(av[2]))
		return (print_error("Invalid time_to_die\n"));
	if (check_arg_content(av[3]) || ft_atoi(av[3]) <= 0 || exceeds_int_max(av[3]))
		return (print_error("Invalid time_to_eat\n"));
	if (check_arg_content(av[4]) || ft_atoi(av[4]) <= 0 || exceeds_int_max(av[4]))
		return (print_error("Invalid time_to_sleep\n"));
	if (ac == 6 && (check_arg_content(av[5]) || ft_atoi(av[5]) <= 0 || exceeds_int_max(av[5])))
		return (print_error("Invalid max_meals\n"));
	return (0);
}

int	parse_args(t_simulation *sim, int ac, char **av)
{
	if (check_valid_args(ac, av))
		return (1);
	sim->num_philosophers = ft_atoi(av[1]);
	sim->time_to_die = ft_atoi(av[2]);
	sim->time_to_eat = ft_atoi(av[3]);
	sim->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		sim->max_meals = ft_atoi(av[5]);
	else
		sim->max_meals = -1;

	sim->is_running = 1;

	if (init_mutexes(sim))
		return (print_error("Mutex init failed\n"));
	init_philos(sim);
	return (0);
}
