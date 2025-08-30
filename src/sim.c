#include "philosophers.h"
int sim_run(sim_t *s)
{
    s->start_ms = now_ms();
    s->threads_created = 0;
    {
        int i;

        i = 0;
        while (i < s->n_actors)
        {
            /* initialize last_meal before thread starts to avoid races */
            s->actors[i].last_meal_ms = now_ms();
            if (pthread_create(&s->actors[i].thread, NULL,
                    actor_routine, &s->actors[i]))
            {
                atomic_store(&s->running, 0);
                printf("%sThread create failed for philo %d%s\n",
                    RED, i + 1, RESET);
                sim_teardown(s);
                return 1;
            }
            s->threads_created++;
            i++;
        }
    }
    sim_monitor(s);
    sim_teardown(s);
    return 0;
}
