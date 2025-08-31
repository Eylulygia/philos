#include "philosophers.h"
static int spawn_threads(sim_t *s)
{
    int i;

    s->threads_created = 0;
    /* Set all last_meal timestamps before any thread starts */
    i = 0;
    while (i < s->n_actors)
    {
        s->actors[i].last_meal_ms = s->start_ms;
        i++;
    }
    /* Create threads */
    i = 0;
    while (i < s->n_actors)
    {
        if (pthread_create(&s->actors[i].thread, NULL,
                actor_routine, &s->actors[i]))
        {
            atomic_store(&s->running, 0);
            printf("Thread create failed for philo %d\n", i + 1);
            sim_teardown(s);
            return (1);
        }
        s->actors[i].started = 1;
        s->threads_created++;
        i++;
    }
    return (0);
}

int sim_run(sim_t *s)
{
    s->start_ms = now_ms();
    if (spawn_threads(s))
        return (1);
    sim_monitor(s);
    sim_teardown(s);
    return (0);
}
