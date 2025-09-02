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

void	sleep_for(long long ms, t_simulation *sim)
{
	long long	start;

	start = now_ms();
	while (get_running(sim))
	{
		if (ms_since(start) >= ms)
			break ;
		usleep(50);
	}
}
