#include "philosophers.h"

static int single_actor_step(actor_t *a)
{
    pthread_mutex_lock(&a->sim->forks[a->left]);
    log_event(a->sim, a->id, "has taken a fork");
    sleep_for(a->sim->t_die, a->sim);
    pthread_mutex_unlock(&a->sim->forks[a->left]);
    return (1);
}

static void lock_forks(actor_t *a, int *first, int *second)
{
    int l;
    int r;

    l = a->left;
    r = a->right;
    if (l < r)
    {
        *first = l;
        *second = r;
    }
    else
    {
        *first = r;
        *second = l;
    }
    pthread_mutex_lock(&a->sim->forks[*first]);
    log_event(a->sim, a->id, "has taken a fork");
    pthread_mutex_lock(&a->sim->forks[*second]);
    log_event(a->sim, a->id, "has taken a fork");
}

int actor_eat(actor_t *a)
{
    int first;
    int second;

    if (a->sim->n_actors == 1)
        return (single_actor_step(a));
    lock_forks(a, &first, &second);
    log_event(a->sim, a->id, "is eating");
    pthread_mutex_lock(&a->sim->guard);
    a->last_meal_ms = now_ms();
    pthread_mutex_unlock(&a->sim->guard);
    sleep_for(a->sim->t_eat, a->sim);
    pthread_mutex_lock(&a->sim->guard);
    a->last_meal_ms = now_ms();
    a->meals++;
    pthread_mutex_unlock(&a->sim->guard);
    pthread_mutex_unlock(&a->sim->forks[first]);
    pthread_mutex_unlock(&a->sim->forks[second]);
    return (0);
}
