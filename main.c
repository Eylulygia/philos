/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahyildir <ahyildir@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:49:52 by ahyildir          #+#    #+#             */
/*   Updated: 2024/06/27 15:49:53 by ahyildir         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <string.h>

int	main(int ac, char **av)
{
    sim_t	sim;

    if (ac == 2 && (!strcmp(av[1], "--help") || !strcmp(av[1], "-h")))
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
        panicf("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_meals]\n");
    if (args_parse(&sim, ac, av))
        panicf("An error occurred while parsing input!\n");
    if (sim_run(&sim))
        panicf("An error occurred while starting the simulation!\n");
    return (0);
}
