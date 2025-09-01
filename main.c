#include "philosophers.h"
/* use local ft_strcmp instead of strcmp */

int	main(int ac, char **av)
{
    simulation_t	 sim;

    if (ac == 2 && (ft_strcmp(av[1], "--help") == 0 || ft_strcmp(av[1], "-h") == 0))
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
        return print_error("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_meals]\n");
    if (parse_args(&sim, ac, av))
        return (1);
    if (run_simulation(&sim))
        return print_error("An error occurred while starting the simulation!\n");
    return (0);
}
