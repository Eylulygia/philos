/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/07 17:07:49 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	if (!p || !p->sim)
		return (NULL);
	pthread_mutex_lock(&p->sim->data_lock);
	p->last_meal_ms = now_ms();
	pthread_mutex_unlock(&p->sim->data_lock);
	if (p->id % 2 == 1)
		sleep_for(p->sim->time_to_eat / 2, p->sim);
	while (get_running(p->sim))
	{
		if (eat_once(p))
			return (NULL);
		log_event(p->sim, p->id, "is sleeping");
		sleep_for(p->sim->time_to_sleep, p->sim);
		log_event(p->sim, p->id, "is thinking");
		usleep(100);
	}
	return (NULL);
}
