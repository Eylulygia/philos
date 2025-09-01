#include "philosophers.h"

void *actor_routine(void *arg)
{
    actor_t *a;

    a = (actor_t *)arg;
    if (a->id % 2 == 1)
        usleep(15000);
    while (1)
    {
        int running;
        int all_full;
        pthread_mutex_lock(&a->sim->guard);
        running = a->sim->running;
        all_full = a->sim->all_full;
        pthread_mutex_unlock(&a->sim->guard);
        if (!running || all_full == 0)
            break ;
        if (actor_eat(a))
            return (NULL);
        log_event(a->sim, a->id, "is sleeping");
        sleep_for(a->sim->t_sleep, a->sim);
        log_event(a->sim, a->id, "is thinking");
    }
    return (NULL);
}
