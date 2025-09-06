/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftime.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:33:54 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 18:43:03 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

long long	ms_since(long long past)
{
	return (now_ms() - past);
}

void    sleep_for(long long ms, t_simulation *sim)
{
    long long end = now_ms() + ms;

    while (get_running(sim))
    {
        long long rem = end - now_ms();
        if (rem <= 0)
            break;

        if (rem > 10) {
            /* Kalan sürenin ~%80'i kadar uyu: aşırı-uyuma riskini azaltır */
            /* rem ms -> usleep mikrosaniye */
            long long chunk_us = (rem * 1000LL * 8) / 10;   /* 0.8 * rem ms */
            if (chunk_us > 2000)            /* 2ms üstünde ise kısalt */
                chunk_us = 2000;            /* 2ms bloklarla ilerle */
            usleep((useconds_t)chunk_us);
        }
        else if (rem > 2) {
            /* 3–10ms arası: küçük bloklar */
            usleep(1000);                   /* 1ms */
        }
        else if (rem > 1) {
            usleep(500);                    /* 0.5ms */
        }
        else {
            /* < =1ms: çok ince adımlar */
            usleep(50);                     /* 0.05ms */
        }
    }
}

