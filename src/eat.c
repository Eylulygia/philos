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
    /* Deterministic order: always lock lower index first to avoid deadlock */
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

static int begin_eating(actor_t *a, int first, int second)
{
    pthread_mutex_lock(&a->sim->guard);
    if (atomic_load(&a->sim->running) == 0)
    {
        pthread_mutex_unlock(&a->sim->guard);
        pthread_mutex_unlock(&a->sim->forks[first]);
        pthread_mutex_unlock(&a->sim->forks[second]);
        return (1);
    }
    a->last_meal_ms = now_ms();
    pthread_mutex_unlock(&a->sim->guard);
    log_event(a->sim, a->id, "is eating");
    return (0);
}

static void finish_eating(actor_t *a, int first, int second)
{
    sleep_for(a->sim->t_eat, a->sim);
    pthread_mutex_lock(&a->sim->guard);
    a->meals++;
    pthread_mutex_unlock(&a->sim->guard);
    pthread_mutex_unlock(&a->sim->forks[first]);
    pthread_mutex_unlock(&a->sim->forks[second]);
}

int actor_eat(actor_t *a)
{
    int first;
    int second;

    if (a->sim->n_actors == 1)
        return (single_actor_step(a));
    lock_forks(a, &first, &second);
    if (begin_eating(a, first, second))
        return (1);
    finish_eating(a, first, second);
    return (0);
}
