/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:38:30 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 16:37:03 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	init_all_forks(t_simulation *sim, int *forks_inited)
{
	int	i;

	*forks_inited = 0;
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL))
			return (1);
		(*forks_inited)++;
		i++;
	}
	return (0);
}

static int	init_post_forks(t_simulation *sim, int forks_inited)
{
	if (pthread_mutex_init(&sim->log_lock, NULL))
	{
		cleanup(sim, forks_inited, 0);
		return (1);
	}
	if (pthread_mutex_init(&sim->data_lock, NULL))
	{
		cleanup(sim, forks_inited, 1);
		return (1);
	}
	if (pthread_mutex_init(&sim->state_lock, NULL))
	{
		cleanup(sim, forks_inited, 1 | 2);
		return (1);
	}
	return (0);
}

int	init_mutexes(t_simulation *sim)
{
	int	forks_inited;

	if (alloc_structs(sim))
		return (1);
	forks_inited = 0;
	if (init_all_forks(sim, &forks_inited))
	{
		cleanup(sim, forks_inited, 0);
		return (1);
	}
	return (init_post_forks(sim, forks_inited));
}
