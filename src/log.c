#include "philosophers.h"

void panicf(const char *msg)
{
    printf("%s%s%s", RED, msg, RESET);
    exit(EXIT_FAILURE);
}

void log_event(sim_t *sim, int actor_id, const char *msg)
{
    long long t = ms_since(sim->start_ms);
    pthread_mutex_lock(&sim->log_lock);
    if (atomic_load(&sim->running))
        printf("%s%lld%s %s%d%s %s\n", RED, t, RESET, GREEN, actor_id + 1, RESET, msg);
    pthread_mutex_unlock(&sim->log_lock);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <you@student.42istanbul.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 22:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/08/30 22:00:00 by yourlogin        ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */
