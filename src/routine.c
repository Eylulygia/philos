/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:39:13 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:39:16 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	if (!p || !p->sim)
		return (NULL);
	if (p->id % 2 == 1)
		usleep(15000);
	while (get_running(p->sim))
	{
		if (get_someone_hungry(p->sim) == 0 || get_running(p->sim) == 0)
			break ;
		if (eat_once(p))
			return (NULL);
		log_event(p->sim, p->id, "is sleeping");
		sleep_for(p->sim->time_to_sleep, p->sim);
		log_event(p->sim, p->id, "is thinking");
	}
	return (NULL);
}
