#include "philosophers.h"
/* Avoid using strcmp; implement a tiny comparator */
static int str_eq(const char *a, const char *b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

int	main(int ac, char **av)
{
    sim_t	sim;

    if (ac == 2 && (str_eq(av[1], "--help") || str_eq(av[1], "-h")))
    {
        printf("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_meals]\n");
        printf("  number_of_philos: >= 1\n");
        printf("  time_to_die: > 0 ms\n");
        printf("  time_to_eat: > 0 ms\n");
        printf("  time_to_sleep: > 0 ms\n");
        printf("  max_meals: optional, >= 0; omit for unlimited\n");
        return (0);
    }
    if (ac != 5 && ac != 6)
        return errorf("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_meals]\n");
    if (args_parse(&sim, ac, av))
        return (1);
    if (sim_run(&sim))
        return errorf("An error occurred while starting the simulation!\n");
    return (0);
}
