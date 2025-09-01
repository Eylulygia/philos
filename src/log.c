#include "philosophers.h"
#include <string.h>

int print_error(const char *msg)
{
    printf("%s%s%s", RED, msg, RESET);
    return (1);
}

void log_event(simulation_t *sim, int philo_id, const char *msg)
{
    long long t = ms_since(sim->start_ms);
    pthread_mutex_lock(&sim->log_lock);
    if (get_running(sim) || strcmp(msg, "died") == 0)
        printf("%s%lld%s %s%d%s %s\n", RED, t, RESET, GREEN, philo_id + 1, RESET, msg);
    pthread_mutex_unlock(&sim->log_lock);
}
