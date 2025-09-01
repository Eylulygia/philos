#include "philosophers.h"

static void check_death(simulation_t *s)
{
    s->temp_index = -1;
    while (++s->temp_index < s->num_philosophers && get_running(s))
    {
        int died = 0;
        pthread_mutex_lock(&s->data_lock);
        if (ms_since(s->philosophers[s->temp_index].last_meal_ms) > s->time_to_die)
            died = 1;
        pthread_mutex_unlock(&s->data_lock);
        if (died && mark_dead(s))
        {
            log_event(s, s->temp_index, "died");
            break;
        }
        usleep(100);
    }
}

static void check_full(simulation_t *s)
{
    s->temp_index = 0;
    while (s->temp_index < s->num_philosophers && s->max_meals != -1)
    {
        int meals;

        pthread_mutex_lock(&s->data_lock);
        meals = s->philosophers[s->temp_index].meals;
        pthread_mutex_unlock(&s->data_lock);
        if (meals < s->max_meals)
            break ;
        s->temp_index++;
    }
    if (s->temp_index == s->num_philosophers)
        set_someone_hungry(s, 0);
}

void monitor_simulation(simulation_t *s)
{
    while (get_someone_hungry(s))
    {
        check_death(s);
        if (get_running(s) == 0)
            break ;
        check_full(s);
    }
}
