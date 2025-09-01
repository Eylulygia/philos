#include "philosophers.h"

static void check_death(sim_t *s)
{
    s->idx = -1;
    while (++s->idx < s->n_actors)
    {
        int running;
        pthread_mutex_lock(&s->guard);
        running = s->running;
        pthread_mutex_unlock(&s->guard);
        if (!running)
            break;
        pthread_mutex_lock(&s->guard);
        if (ms_since(s->actors[s->idx].last_meal_ms) > s->t_die)
        {
            s->running = 0;
            pthread_mutex_unlock(&s->guard);
            log_event(s, s->idx, "died");
        }
        else
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
    {
        pthread_mutex_lock(&s->guard);
        s->all_full = 0;
        pthread_mutex_unlock(&s->guard);
    }
}

void sim_monitor(sim_t *s)
{
    while (1)
    {
        int all_full;
        pthread_mutex_lock(&s->guard);
        all_full = s->all_full;
        pthread_mutex_unlock(&s->guard);
        if (!all_full)
            break;
        check_death(s);
        int running;
        pthread_mutex_lock(&s->guard);
        running = s->running;
        pthread_mutex_unlock(&s->guard);
        if (running == 0)
            break ;
        check_full(s);
    }
}
