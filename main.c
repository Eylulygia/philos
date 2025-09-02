/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:29:33 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:33:43 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_simulation	sim;

	if (ac == 2 && (ft_strcmp(av[1], "--help") == 0 || ft_strcmp(av[1],
				"-h") == 0))
	{
		printf("numberphilos timetodie timeeat timesleep [max_meals]\n");
		printf("  number_of_philos: >= 1\n");
		printf("  time_to_die: > 0 ms\n");
		printf("  time_to_eat: > 0 ms\n");
		printf("  time_to_sleep: > 0 ms\n");
		printf("  max_meals: optional, >= 0; omit for unlimited\n");
		return (0);
	}
	if (ac != 5 && ac != 6)
		return (print_error("numberphilo timedie timeat timesleep [maxmeal]\n"));
	if (parse_args(&sim, ac, av))
		return (1);
	if (run_simulation(&sim))
		return (print_error("error occurred while starting the simulation!\n"));
	return (0);
}
