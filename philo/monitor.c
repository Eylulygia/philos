/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/06 18:30:00 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	check_death(t_simulation *s)
{
	int	died;

	s->temp_index = -1;
	while (++s->temp_index < s->num_philosophers && get_running(s))
	{
		died = 0;
		pthread_mutex_lock(&s->data_lock);
		if (ms_since(s->philosophers[s->temp_index].last_meal_ms)
			> s->time_to_die)
			died = 1;
		pthread_mutex_unlock(&s->data_lock);
		if (died && mark_dead(s))
		{
			/* ID’yi filo objesinden al: 0/1 taban farkı olmasın */
			log_event(s, s->philosophers[s->temp_index].id, "died");
			break ;
		}
		usleep(100);
	}
}

static int	check_full(t_simulation *s)
{
	int	i;

	if (s->max_meals < 0)
		return (0);
	pthread_mutex_lock(&s->data_lock);
	i = 0;
	while (i < s->num_philosophers)
	{
		if (s->philosophers[i].meals < s->max_meals)
		{
			pthread_mutex_unlock(&s->data_lock);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&s->data_lock);
	return (1);
}


void	monitor_simulation(t_simulation *s)
{
	while (get_running(s))
	{
		check_death(s);
		if (!get_running(s))
			break ;
		if (check_full(s))
		{
			set_running(s, 0);
			break ;
		}
		usleep(500);
	}
}
