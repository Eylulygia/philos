/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/06 18:30:00 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	spawn_threads(t_simulation *s)
{
	int	i;

	s->threads_created = 0;
	i = 0;
	while (i < s->num_philosophers)
	{
		s->philosophers[i].last_meal_ms = s->start_ms;
		i++;
	}
	i = 0;
	while (i < s->num_philosophers)
	{
		if (pthread_create(&s->philosophers[i].thread, NULL,
				philosopher_routine, &s->philosophers[i]))
		{
			set_running(s, 0);
			printf("Thread create failed for philo %d\n", i + 1);
			teardown_simulation(s);
			return (1);
		}
		s->philosophers[i].thread_started = 1;
		s->threads_created++;
		i++;
	}
	return (0);
}

int	run_simulation(t_simulation *s)
{
	s->start_ms = now_ms();
	if (spawn_threads(s))
		return (1);
	monitor_simulation(s);
	teardown_simulation(s);
	return (0);
}

int	main(int ac, char **av)
{
	t_simulation	sim;

	if (ac != 5 && ac != 6)
		return (print_error("numberphilo timedie timeat timesleep [maxmeal]\n"));
	if (parse_args(&sim, ac, av))
		return (1);
	if (run_simulation(&sim))
		return (print_error("error occurred while starting the simulation!\n"));
	return (0);
}

