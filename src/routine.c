#include "philosophers.h"

void *actor_routine(void *arg)
{
    actor_t *a;
    a = (actor_t *)arg;
    if (!a || !a->sim)
        return (NULL);
    if (a->id % 2 == 1)
        usleep(15000);
    while (atomic_load(&a->sim->running))
    {
        if (atomic_load(&a->sim->all_full) == 0
            || atomic_load(&a->sim->running) == 0)
            break ;
        if (actor_eat(a))
            return (NULL);
        log_event(a->sim, a->id, "is sleeping");
        sleep_for(a->sim->t_sleep, a->sim);
        log_event(a->sim, a->id, "is thinking");
    }
    return (NULL);
}
