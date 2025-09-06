/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:30:00 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/06 18:30:00 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((int)((unsigned char)s1[i]) - (int)((unsigned char)s2[i]));
}

int	get_running(t_simulation *s)
{
	int v;
	pthread_mutex_lock(&s->data_lock);
	v = s->is_running;
	pthread_mutex_unlock(&s->data_lock);
	return v;
}

void	set_running(t_simulation *s, int v)
{
	pthread_mutex_lock(&s->data_lock);
	s->is_running = v;
	pthread_mutex_unlock(&s->data_lock);
}

int	mark_dead(t_simulation *s)
{
	int changed = 0;
	pthread_mutex_lock(&s->data_lock);
	if (s->is_running)
	{
		s->is_running = 0;
		changed = 1;
	}
	pthread_mutex_unlock(&s->data_lock);
	return changed;
}