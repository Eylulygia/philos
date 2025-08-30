/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <you@student.42istanbul.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 22:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/08/30 22:00:00 by yourlogin        ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *actor_routine(void *arg)
{
    actor_t *a;

    a = (actor_t *)arg;
    if (a->id % 2 == 1)
        usleep(15000);
    while (atomic_load(&a->sim->running))
    {
        if (atomic_load(&a->sim->all_full) == 0
            || atomic_load(&a->sim->running) == 0)
            break ;
        if (actor_eat(a))
            return (NULL);
        log_event(a->sim, a->id, "is sleeping");
        sleep_for(a->sim->t_sleep, a->sim);
        log_event(a->sim, a->id, "is thinking");
    }
    return (NULL);
}

