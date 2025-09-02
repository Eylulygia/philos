/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:39:01 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:39:04 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	skip_space_and_sign(const char *s, const char **outp)
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

static int	parse_u32(const char *s, long *out)
{
	const char	*p;
	long		val;
	int			len;

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

static int	report_parse_error(int rc)
{
	if (rc == 1)
		return (print_error("Only positive integers are allowed.\n"));
	if (rc == 2)
		return (print_error("Out of range: use [0..INT_MAX].\n"));
	if (rc == 3)
		return (print_error("Invalid character: only digits 0-9 allowed.\n"));
	return (0);
}

static int	parse_one(const char *s, long *out)
{
	int	rc;

	rc = parse_u32(s, out);
	if (report_parse_error(rc))
		return (1);
	return (0);
}

int	parse_fields(simulation_t *sim, int ac, char **av)
{
	long	v;

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
