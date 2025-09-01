#include "philosophers.h"

/* internal helpers for partial init teardown */
static void destroy_forks(simulation_t *sim, int count)
{
    for (int i = 0; i < count; i++)
        pthread_mutex_destroy(&sim->forks[i]);
}

static void cleanup(simulation_t *sim, int forks_inited, int log_inited, int data_inited, int state_inited)
{
    if (data_inited)
        pthread_mutex_destroy(&sim->data_lock);
    if (state_inited)
        pthread_mutex_destroy(&sim->state_lock);
    if (log_inited)
        pthread_mutex_destroy(&sim->log_lock);
    destroy_forks(sim, forks_inited);
    free(sim->philosophers);
    free(sim->forks);
}

int alloc_structs(simulation_t *sim)
{
    sim->philosophers = malloc(sizeof(philosopher_t) * sim->num_philosophers);
    if (!sim->philosophers)
        return (1);
    sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
    if (!sim->forks)
    {
        free(sim->philosophers);
        return (1);
    }
    return (0);
}

int init_all_forks(simulation_t *sim, int *forks_inited)
{
    int i;

    *forks_inited = 0;
    i = 0;
    while (i < sim->num_philosophers)
    {
        if (pthread_mutex_init(&sim->forks[i], NULL))
            return (1);
        (*forks_inited)++;
        i++;
    }
    return (0);
}

int init_mutexes(simulation_t *sim)
{
    int forks_inited;

    if (alloc_structs(sim))
        return (1);
    forks_inited = 0;
    if (init_all_forks(sim, &forks_inited))
    {
        cleanup(sim, forks_inited, 0, 0, 0);
        return (1);
    }
    if (pthread_mutex_init(&sim->log_lock, NULL))
    {
        cleanup(sim, forks_inited, 0, 0, 0);
        return (1);
    }
    if (pthread_mutex_init(&sim->data_lock, NULL))
    {
        cleanup(sim, forks_inited, 1, 0, 0);
        return (1);
    }
    if (pthread_mutex_init(&sim->state_lock, NULL))
    {
        cleanup(sim, forks_inited, 1, 1, 0);
        return (1);
    }
    return (0);
}

void init_philos(simulation_t *sim)
{
    int i = 0;
    while (i < sim->num_philosophers)
    {
        sim->philosophers[i].id = i;
        sim->philosophers[i].right_fork = i;
        sim->philosophers[i].left_fork = (i + 1) % sim->num_philosophers;
        sim->philosophers[i].last_meal_ms = 0;
        sim->philosophers[i].meals = 0;
        sim->philosophers[i].thread_started = 0;
        sim->philosophers[i].sim = sim;
        i++;
    }
}
