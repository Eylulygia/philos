#include "philosophers.h"

static void check_death(sim_t *s)
{
    s->idx = -1;
    while (++s->idx < s->n_actors && atomic_load(&s->running))
    {
        pthread_mutex_lock(&s->guard);
        if (ms_since(s->actors[s->idx].last_meal_ms) > s->t_die)
        {
            log_event(s, s->idx, "died");
            atomic_store(&s->running, 0);
        }
        pthread_mutex_unlock(&s->guard);
        usleep(100);
    }
}

static void check_full(sim_t *s)
{
    s->idx = 0;
    while (s->idx < s->n_actors && s->max_meals != -1)
    {
        int meals;

        pthread_mutex_lock(&s->guard);
        meals = s->actors[s->idx].meals;
        pthread_mutex_unlock(&s->guard);
        if (meals < s->max_meals)
            break ;
        s->idx++;
    }
    if (s->idx == s->n_actors)
        atomic_store(&s->all_full, 0);
}

void sim_monitor(sim_t *s)
{
    while (atomic_load(&s->all_full))
    {
        check_death(s);
        if (atomic_load(&s->running) == 0)
            break ;
        check_full(s);
    }
}
