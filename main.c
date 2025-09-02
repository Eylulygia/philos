/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:29:33 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 18:10:16 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_simulation	sim;

	if (ac != 5 && ac != 6)
		return (print_error("numberphilo timedie timeat timesleep [maxmeal]\n"));
	if (parse_args(&sim, ac, av))
		return (1);
	if (run_simulation(&sim))
		return (print_error("error occurred while starting the simulation!\n"));
	return (0);
}
