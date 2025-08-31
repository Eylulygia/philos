#include "philosophers.h"

void sim_teardown(sim_t *s)
{
    int i;

    i = 0;
    while (i < s->n_actors)
    {
        if (s->actors[i].started)
            pthread_join(s->actors[i].thread, NULL);
        i++;
    }
    i = 0;
    while (i < s->n_actors)
    {
        pthread_mutex_destroy(&s->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&s->log_lock);
    pthread_mutex_destroy(&s->guard);
    free(s->actors);
    free(s->forks);
    s->threads_created = 0;
}
