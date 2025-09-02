/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:40:29 by ekamar            #+#    #+#             */
/*   Updated: 2025/09/02 15:41:07 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

typedef struct s_simulation	simulation_t;

typedef struct s_philosopher
{
	int						id;
	int						left_fork;
	int						right_fork;
	int						meals;
	long long				last_meal_ms;
	pthread_t				thread;
	int						thread_started;
	simulation_t			*sim;
}							philosopher_t;

struct						s_simulation
{
	int						num_philosophers;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						max_meals;

	int						is_running;
	int						someone_hungry;
	int						temp_index;
	int						threads_created;

	long long				start_ms;
	pthread_mutex_t			*forks;
	pthread_mutex_t			log_lock;
	pthread_mutex_t			data_lock;
	pthread_mutex_t			state_lock;

	philosopher_t			*philosophers;
};

int							parse_args(simulation_t *sim, int ac, char **av);
int							parse_fields(simulation_t *sim, int ac, char **av);
int							run_simulation(simulation_t *sim);
void						*philosopher_routine(void *arg);
int							eat_once(philosopher_t *p);
void						monitor_simulation(simulation_t *s);
void						teardown_simulation(simulation_t *s);

int							get_running(simulation_t *s);
void						set_running(simulation_t *s, int v);
int							get_someone_hungry(simulation_t *s);
void						set_someone_hungry(simulation_t *s, int v);
int							mark_dead(simulation_t *s);

long long					now_ms(void);
long long					ms_since(long long past);
void						sleep_for(long long ms, simulation_t *sim);

int							print_error(const char *msg);
void						log_event(simulation_t *sim, int philo_id,
								const char *msg);

int							ft_strcmp(const char *s1, const char *s2);

int							init_mutexes(simulation_t *sim);
void						init_philos(simulation_t *sim);
int							alloc_structs(simulation_t *sim);

#endif
