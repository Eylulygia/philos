/* Public interface for the refactored Philosophers project */
#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdatomic.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

typedef struct s_sim sim_t;

typedef struct s_actor
{
    int         id;
    int         left;
    int         right;
    int         meals;
    long long   last_meal_ms;
    pthread_t   thread;
    int         started;
    sim_t      *sim;
} actor_t;

struct s_sim
{
    int             n_actors;
    int             t_die;
    int             t_eat;
    int             t_sleep;
    int             max_meals; /* -1 unlimited */

    atomic_int      running;   /* 1 while sim is active */
    atomic_int      all_full;  /* 0 when everyone reached max_meals */
    int             idx;       /* scratch index for monitors */
    int             threads_created;

    long long       start_ms;
    pthread_mutex_t *forks;
    pthread_mutex_t log_lock;
    pthread_mutex_t guard;     /* protects last_meal_ms writes */

    actor_t        *actors;
};

/* args */
int         args_parse(sim_t *sim, int ac, char **av);

/* sim */
int         sim_run(sim_t *sim);
void       *actor_routine(void *arg);
int         actor_eat(actor_t *a);
void        sim_monitor(sim_t *s);
void        sim_teardown(sim_t *s);

/* time.c */
long long   now_ms(void);
long long   ms_since(long long past);
void        sleep_for(long long ms, sim_t *sim);

/* log.c */
int         errorf(const char *msg);
void        log_event(sim_t *sim, int actor_id, const char *msg);

#endif
