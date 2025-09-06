/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:36:54 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:37:51 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	validate(t_simulation *sim, int ac)
{
	if (sim->num_philosophers < 1)
		return (print_error("Invalid number_of_philos (must be >= 1).\n"));
	if (sim->time_to_die <= 0)
		return (print_error("Invalid time_to_die (must be > 0 ms).\n"));
	if (sim->time_to_eat <= 0)
		return (print_error("Invalid time_to_eat (must be > 0 ms).\n"));
	if (sim->time_to_sleep <= 0)
		return (print_error("Invalid time_to_sleep (must be > 0 ms).\n"));
	if (ac == 6 && sim->max_meals < 0)
		return (print_error("Invalid max_meals.\n"));
	return (0);
}

int	parse_args(t_simulation *sim, int ac, char **av)
{
    if (parse_fields(sim, ac, av))
        return (1);
    sim->is_running = 1;
    sim->someone_hungry = 1;
    if (validate(sim, ac))
        return (1);
    if (init_mutexes(sim))
        return (print_error("error occured during mutex initialization\n"));
    init_philos(sim);
    return (0);
}

void	init_philos(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i;
		sim->philosophers[i].right_fork = i;
		sim->philosophers[i].left_fork = (i + 1) % sim->num_philosophers;
		sim->philosophers[i].last_meal_ms = 0;
		sim->philosophers[i].meals = 0;
		sim->philosophers[i].thread_started = 0;
		sim->philosophers[i].sim = sim;
		i++;
	}
}

int	alloc_structs(t_simulation *sim)
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	if (!sim->philosophers)
		return (1);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		free(sim->philosophers);
		return (1);
	}
	return (0);
}
