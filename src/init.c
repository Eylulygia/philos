/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:38:30 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:42:47 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_forks(simulation_t *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
}

static void	cleanup(simulation_t *sim, int forks_inited, int log_inited,
		int data_inited, int state_inited)
{
	if (data_inited)
		pthread_mutex_destroy(&sim->data_lock);
	if (state_inited)
		pthread_mutex_destroy(&sim->state_lock);
	if (log_inited)
		pthread_mutex_destroy(&sim->log_lock);
	destroy_forks(sim, forks_inited);
	free(sim->philosophers);
	free(sim->forks);
}

int	init_all_forks(simulation_t *sim, int *forks_inited)
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

static int	init_post_forks(simulation_t *sim, int forks_inited)
{
	if (pthread_mutex_init(&sim->log_lock, NULL))
	{
		cleanup(sim, forks_inited, 0, 0, 0);
		return (1);
	}
	if (pthread_mutex_init(&sim->data_lock, NULL))
	{
		cleanup(sim, forks_inited, 1, 0, 0);
		return (1);
	}
	if (pthread_mutex_init(&sim->state_lock, NULL))
	{
		cleanup(sim, forks_inited, 1, 1, 0);
		return (1);
	}
	return (0);
}

int	init_mutexes(simulation_t *sim)
{
	int	forks_inited;

	if (alloc_structs(sim))
		return (1);
	forks_inited = 0;
	if (init_all_forks(sim, &forks_inited))
	{
		cleanup(sim, forks_inited, 0, 0, 0);
		return (1);
	}
	return (init_post_forks(sim, forks_inited));
}
