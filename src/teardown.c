/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teardown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:39:42 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:39:46 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	teardown_simulation(simulation_t *s)
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
