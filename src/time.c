#include "philosophers.h"

long long now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

long long ms_since(long long past)
{
    return now_ms() - past;
}

void sleep_for(long long ms, sim_t *sim)
{
    long long start = now_ms();
    while (atomic_load(&sim->running))
    {
        if (ms_since(start) >= ms)
            break;
        usleep(50);
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <you@student.42istanbul.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 22:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/08/30 22:00:00 by yourlogin        ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */
