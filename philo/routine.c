/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/06 18:30:00 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	smart_think(t_philosopher *p)
{
	t_simulation *sim = p->sim;
	long long margin, think_ms;

	if (sim->num_philosophers >= 100)
		return;

	margin = (long long)sim->time_to_die
		   - ((long long)sim->time_to_eat + (long long)sim->time_to_sleep);

	if (margin > 0)
	{
		think_ms = margin / 2;
		if (think_ms > 50)
			think_ms = 50;
		if (think_ms < 1)
			think_ms = 1;
		sleep_for(think_ms, sim);
	}
	else
		sleep_for(1, sim);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	if (!p || !p->sim)
		return (NULL);
	/* başlatırken güvenli ilk yemek zamanı damgası */
	pthread_mutex_lock(&p->sim->data_lock);
	p->last_meal_ms = now_ms();
	pthread_mutex_unlock(&p->sim->data_lock);

	/* tek tıkanmayı önlemek için başlangıçta küçük bekleme */
	if (p->id % 2 == 1)
		sleep_for(p->sim->time_to_eat / 2, p->sim);

	while (get_running(p->sim))
	{
		if (eat_once(p))
			return (NULL);
		log_event(p->sim, p->id, "is sleeping");
		sleep_for(p->sim->time_to_sleep, p->sim);
		log_event(p->sim, p->id, "is thinking");
		smart_think(p);
	}
	return (NULL);
}
