#include "philosophers.h"

/* return 0 ok, 1 negative sign; sets *outp */
static int skip_space_and_sign(const char *s, const char **outp)
{
    while ((*s >= 9 && *s <= 13) || *s == 32)
        s++;
    if (*s == '+')
        s++;
    else if (*s == '-')
        return (1);
    *outp = s;
    return (0);
}

/* return 0 ok, 1 negative, 2 overflow, 3 invalid-char */
static int parse_u32(const char *s, long *out)
{
    const char *p;
    long        val;
    int         len;

    if (skip_space_and_sign(s, &p))
        return (1);
    val = 0;
    len = 0;
    while (p[len] >= '0' && p[len] <= '9')
    {
        val = val * 10 + (p[len] - '0');
        if (val > INT_MAX)
            return (2);
        len++;
    }
    if (!(p[len] == '\0' || (p[len] >= 9 && p[len] <= 13) || p[len] == 32))
        return (3);
    *out = val;
    return (0);
}

static int validate(sim_t *sim, int ac)
{
    if (sim->n_actors < 1)
        return errorf("Invalid number_of_philos (must be >= 1).\n");
    if (sim->t_die <= 0)
        return errorf("Invalid time_to_die (must be > 0 ms).\n");
    if (sim->t_eat <= 0)
        return errorf("Invalid time_to_eat (must be > 0 ms).\n");
    if (sim->t_sleep <= 0)
        return errorf("Invalid time_to_sleep (must be > 0 ms).\n");
    if (ac == 6 && sim->max_meals < 0)
        return errorf("Invalid max_meals (must be >= 0, or omit for unlimited).\n");
    return (0);
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
    {
        long v;
        int  rc;

        rc = parse_u32(av[1], &v);
        if (rc == 1)
            return errorf("Only positive integers are allowed.\n");
        if (rc == 2)
            return errorf("Out of range: use [0..INT_MAX].\n");
        if (rc == 3)
            return errorf("Invalid character: only digits 0-9 allowed.\n");
        sim->n_actors = v;
    }
    {
        long v; int rc = parse_u32(av[2], &v);
        if (rc == 1)
            return errorf("Only positive integers are allowed.\n");
        if (rc == 2)
            return errorf("Out of range: use [0..INT_MAX].\n");
        if (rc == 3)
            return errorf("Invalid character: only digits 0-9 allowed.\n");
        sim->t_die = v;
    }
    {
        long v; int rc = parse_u32(av[3], &v);
        if (rc == 1)
            return errorf("Only positive integers are allowed.\n");
        if (rc == 2)
            return errorf("Out of range: use [0..INT_MAX].\n");
        if (rc == 3)
            return errorf("Invalid character: only digits 0-9 allowed.\n");
        sim->t_eat = v;
    }
    {
        long v; int rc = parse_u32(av[4], &v);
        if (rc == 1)
            return errorf("Only positive integers are allowed.\n");
        if (rc == 2)
            return errorf("Out of range: use [0..INT_MAX].\n");
        if (rc == 3)
            return errorf("Invalid character: only digits 0-9 allowed.\n");
        sim->t_sleep = v;
    }
    atomic_store(&sim->running, 1);
    atomic_store(&sim->all_full, 1);
    if (ac == 6)
    {
        long v; int rc = parse_u32(av[5], &v);
        if (rc == 1)
            return errorf("Only positive integers are allowed.\n");
        if (rc == 2)
            return errorf("Out of range: use [0..INT_MAX].\n");
        if (rc == 3)
            return errorf("Invalid character: only digits 0-9 allowed.\n");
        sim->max_meals = v;
    }
    else
        sim->max_meals = -1;
    validate(sim, ac);
    if (init_mutexes(sim))
        return errorf("An error occured during mutex initialization, try again.\n");
    init_actors(sim);
    return (0);
}
