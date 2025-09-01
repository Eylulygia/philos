#include "philosophers.h"

 
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

static int report_parse_error(int rc)
{
    if (rc == 1)
        return print_error("Only positive integers are allowed.\n");
    if (rc == 2)
        return print_error("Out of range: use [0..INT_MAX].\n");
    if (rc == 3)
        return print_error("Invalid character: only digits 0-9 allowed.\n");
    return (0);
}

static int parse_one(const char *s, long *out)
{
    int rc = parse_u32(s, out);
    if (report_parse_error(rc))
        return (1);
    return (0);
}

 
static int parse_fields(simulation_t *sim, int ac, char **av)
{
    long v;

    if (parse_one(av[1], &v))
        return (1);
    sim->num_philosophers = v;
    if (parse_one(av[2], &v))
        return (1);
    sim->time_to_die = v;
    if (parse_one(av[3], &v))
        return (1);
    sim->time_to_eat = v;
    if (parse_one(av[4], &v))
        return (1);
    sim->time_to_sleep = v;
    if (ac == 6)
    {
        if (parse_one(av[5], &v))
            return (1);
        sim->max_meals = v;
    }
    else
        sim->max_meals = -1;
    return (0);
}

 
static int validate(simulation_t *sim, int ac)
{
    if (sim->num_philosophers < 1)
        return print_error("Invalid number_of_philos (must be >= 1).\n");
    if (sim->time_to_die <= 0)
        return print_error("Invalid time_to_die (must be > 0 ms).\n");
    if (sim->time_to_eat <= 0)
        return print_error("Invalid time_to_eat (must be > 0 ms).\n");
    if (sim->time_to_sleep <= 0)
        return print_error("Invalid time_to_sleep (must be > 0 ms).\n");
    if (ac == 6 && sim->max_meals < 0)
        return print_error("Invalid max_meals (must be >= 0, or omit for unlimited).\n");
    return (0);
}

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

static int alloc_structs(simulation_t *sim)
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

static int init_all_forks(simulation_t *sim, int *forks_inited)
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

static int init_mutexes(simulation_t *sim)
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

static void init_philos(simulation_t *sim)
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

int parse_args(simulation_t *sim, int ac, char **av)
{
    if (parse_fields(sim, ac, av))
        return (1);
    sim->is_running = 1;
    sim->someone_hungry = 1;
    validate(sim, ac);
    if (init_mutexes(sim))
        return print_error("An error occured during mutex initialization, try again.\n");
    init_philos(sim);
    return (0);
}
