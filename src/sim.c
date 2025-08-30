/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <you@student.42istanbul.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 22:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/08/30 22:00:00 by yourlogin        ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
int sim_run(sim_t *s)
{
    s->start_ms = now_ms();
    s->threads_created = 0;
    {
        int i;

        i = 0;
        while (i < s->n_actors)
        {
            if (pthread_create(&s->actors[i].thread, NULL,
                    actor_routine, &s->actors[i]))
            {
                atomic_store(&s->running, 0);
                printf("%sThread create failed for philo %d%s\n",
                    RED, i + 1, RESET);
                sim_teardown(s);
                return 1;
            }
            s->actors[i].last_meal_ms = now_ms();
            s->threads_created++;
            i++;
        }
    }
    sim_monitor(s);
    sim_teardown(s);
    return 0;
}
