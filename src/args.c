#include "philosophers.h"

static const char *skip_space_and_sign(const char *s)
{
    while ((*s >= 9 && *s <= 13) || *s == 32)
        s++;
    if (*s == '+')
        s++;
    else if (*s == '-')
        panicf("Only positive integers are allowed.\n");
    return s;
}

static long parse_u32(const char *s)
{
    const char *p = skip_space_and_sign(s);
    long val = 0;
    int  len = 0;
    while (p[len] >= '0' && p[len] <= '9')
    {
        val = val * 10 + (p[len] - '0');
        if (val > INT_MAX)
            panicf("Out of range: use [0..INT_MAX].\n");
        len++;
    }
    if (!(p[len] == '\0' || (p[len] >= 9 && p[len] <= 13) || p[len] == 32))
        panicf("Invalid character: only digits 0-9 allowed.\n");
    return val;
}

static int validate(sim_t *sim, int ac)
{
    if (sim->n_actors < 1)
        panicf("Invalid number_of_philos (must be >= 1).\n");
    if (sim->t_die <= 0)
        panicf("Invalid time_to_die (must be > 0 ms).\n");
    if (sim->t_eat <= 0)
        panicf("Invalid time_to_eat (must be > 0 ms).\n");
    if (sim->t_sleep <= 0)
        panicf("Invalid time_to_sleep (must be > 0 ms).\n");
    if (ac == 6 && sim->max_meals < 0)
        panicf("Invalid max_meals (must be >= 0, or omit for unlimited).\n");
    return 0;
}

static void destroy_forks(sim_t *sim, int count)
{
    for (int i = 0; i < count; i++)
        pthread_mutex_destroy(&sim->forks[i]);
}

static void cleanup(sim_t *sim, int forks_inited, int log_inited, int guard_inited)
{
    if (guard_inited)
        pthread_mutex_destroy(&sim->guard);
    if (log_inited)
        pthread_mutex_destroy(&sim->log_lock);
    destroy_forks(sim, forks_inited);
    free(sim->actors);
    free(sim->forks);
}

static int init_mutexes(sim_t *sim)
{
    int forks_inited = 0;
    sim->actors = malloc(sizeof(actor_t) * sim->n_actors);
    if (!sim->actors)
        return 1;
    sim->forks = malloc(sizeof(pthread_mutex_t) * sim->n_actors);
    if (!sim->forks)
    {
        free(sim->actors);
        return 1;
    }
    {
        int i;

        i = 0;
        while (i < sim->n_actors)
        {
            if (pthread_mutex_init(&sim->forks[i], NULL))
            {
                cleanup(sim, forks_inited, 0, 0);
                return 1;
            }
            forks_inited++;
            i++;
        }
    }
    if (pthread_mutex_init(&sim->log_lock, NULL))
    {
        cleanup(sim, forks_inited, 0, 0);
        return 1;
    }
    if (pthread_mutex_init(&sim->guard, NULL))
    {
        cleanup(sim, forks_inited, 1, 0);
        return 1;
    }
    return 0;
}

static void init_actors(sim_t *sim)
{
    int i;

    i = 0;
    while (i < sim->n_actors)
    {
        sim->actors[i].id = i;
        sim->actors[i].right = i;
        sim->actors[i].left = (i + 1) % sim->n_actors;
        sim->actors[i].last_meal_ms = 0;
        sim->actors[i].meals = 0;
        sim->actors[i].sim = sim;
        i++;
    }
}

int args_parse(sim_t *sim, int ac, char **av)
{
    sim->n_actors = parse_u32(av[1]);
    sim->t_die = parse_u32(av[2]);
    sim->t_eat = parse_u32(av[3]);
    sim->t_sleep = parse_u32(av[4]);
    atomic_store(&sim->running, 1);
    atomic_store(&sim->all_full, 1);
    sim->max_meals = (ac == 6) ? parse_u32(av[5]) : -1;
    validate(sim, ac);
    if (init_mutexes(sim))
        panicf("An error occured during mutex initialization, try again.\n");
    init_actors(sim);
    return 0;
}
