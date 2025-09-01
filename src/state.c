#include "philosophers.h"

int get_running(simulation_t *s)
{
    int v;
    pthread_mutex_lock(&s->state_lock);
    v = s->is_running;
    pthread_mutex_unlock(&s->state_lock);
    return v;
}

void set_running(simulation_t *s, int v)
{
    pthread_mutex_lock(&s->state_lock);
    s->is_running = v;
    pthread_mutex_unlock(&s->state_lock);
}

int get_someone_hungry(simulation_t *s)
{
    int v;
    pthread_mutex_lock(&s->state_lock);
    v = s->someone_hungry;
    pthread_mutex_unlock(&s->state_lock);
    return v;
}

void set_someone_hungry(simulation_t *s, int v)
{
    pthread_mutex_lock(&s->state_lock);
    s->someone_hungry = v;
    pthread_mutex_unlock(&s->state_lock);
}

int mark_dead(simulation_t *s)
{
    int changed = 0;
    pthread_mutex_lock(&s->state_lock);
    if (s->is_running != 0)
    {
        s->is_running = 0;
        changed = 1;
    }
    pthread_mutex_unlock(&s->state_lock);
    return changed;
}
