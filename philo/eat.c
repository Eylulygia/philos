/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:38:07 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 22:46:54 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int single_philo_step(t_philosopher *p)
{
    pthread_mutex_lock(&p->sim->forks[p->left_fork]);
    log_event(p->sim, p->id, "has taken a fork");
    sleep_for(p->sim->time_to_die, p->sim);
    pthread_mutex_unlock(&p->sim->forks[p->left_fork]);
    return (1);
}

static void lock_forks(t_philosopher *p, int *first, int *second)
{
    if ((p->id % 2) == 0)
    {
        *first  = p->left_fork;
        *second = p->right_fork;
    }
    else
    {
        *first  = p->right_fork;
        *second = p->left_fork;
    }

    pthread_mutex_lock(&p->sim->forks[*first]);
    log_event(p->sim, p->id, "has taken a fork");
    pthread_mutex_lock(&p->sim->forks[*second]);
    log_event(p->sim, p->id, "has taken a fork");
}

static int begin_eating(t_philosopher *p, int first, int second)
{
    pthread_mutex_lock(&p->sim->data_lock);
    /* get_running() also locks data_lock; we already hold it. */
    if (p->sim->is_running == 0)
    {
        pthread_mutex_unlock(&p->sim->data_lock);
        pthread_mutex_unlock(&p->sim->forks[first]);
        pthread_mutex_unlock(&p->sim->forks[second]);
        return (1);
    }
    p->last_meal_ms = now_ms();   /* SADECE burada */
    pthread_mutex_unlock(&p->sim->data_lock);
    log_event(p->sim, p->id, "is eating");
    return (0);
}


static void finish_eating(t_philosopher *p, int first, int second)
{
    sleep_for(p->sim->time_to_eat, p->sim);

    pthread_mutex_lock(&p->sim->data_lock);
    p->meals++;
    pthread_mutex_unlock(&p->sim->data_lock);

    pthread_mutex_unlock(&p->sim->forks[first]);
    pthread_mutex_unlock(&p->sim->forks[second]);
}


int eat_once(t_philosopher *p)
{
    int first, second;

    if (p->sim->num_philosophers == 1)
        return (single_philo_step(p));

    lock_forks(p, &first, &second);
    if (begin_eating(p, first, second))
        return (1);
    finish_eating(p, first, second);
    return (0);
}
