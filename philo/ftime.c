/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftime.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:33:54 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/07 17:11:47 by ekamar           ###   ########.fr       */
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

void	sleep_for(long long ms, t_simulation *sim)
{
	long long	end;
	long long	rem;
	long long	chunk_us;

	end = now_ms() + ms;
	while (get_running(sim))
	{
		rem = end - now_ms();
		if (rem <= 0)
			break ;
		if (rem > 10)
		{
			chunk_us = (rem * 1000LL * 8) / 10;
			if (chunk_us > 2000)
				chunk_us = 2000;
			usleep(chunk_us);
		}
		else if (rem > 2)
			usleep(1000);
		else if (rem > 1)
			usleep(500);
		else
			usleep(50);
	}
}
