/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teardown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/06 18:30:00 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_forks(t_simulation *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
}
void	cleanup(t_simulation *sim, int forks_inited, int flags)
{
	if (flags & 2)
		pthread_mutex_destroy(&sim->data_lock);
	if (flags & 4)
		pthread_mutex_destroy(&sim->state_lock);
	if (flags & 1)
		pthread_mutex_destroy(&sim->log_lock);
	destroy_forks(sim, forks_inited);
	free(sim->philosophers);
	free(sim->forks);
}

void	teardown_simulation(t_simulation *s)
{
	int	i;

	i = 0;
	while (i < s->num_philosophers)
	{
		if (s->philosophers[i].thread_started)
			pthread_join(s->philosophers[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < s->num_philosophers)
	{
		pthread_mutex_destroy(&s->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&s->log_lock);
	pthread_mutex_destroy(&s->data_lock);
	pthread_mutex_destroy(&s->state_lock);
	free(s->philosophers);
	free(s->forks);
	s->threads_created = 0;
}
