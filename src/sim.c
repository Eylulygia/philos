#include "philosophers.h"
static int spawn_threads(sim_t *s)
{
    int i;

    s->threads_created = 0;
    i = 0;
    while (i < s->n_actors)
    {
        s->actors[i].last_meal_ms = now_ms();
        if (pthread_create(&s->actors[i].thread, NULL,
                actor_routine, &s->actors[i]))
        {
            pthread_mutex_lock(&s->guard);
            s->running = 0;
            pthread_mutex_unlock(&s->guard);
            printf("%sThread create failed for philo %d%s\n",
                RED, i + 1, RESET);
            sim_teardown(s);
            return (1);
        }
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
