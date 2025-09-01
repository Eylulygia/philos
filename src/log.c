#include "philosophers.h"

int ft_strcmp(const char *s1, const char *s2)
{
    unsigned int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((int)((unsigned char)s1[i]) - (int)((unsigned char)s2[i]));
}

int print_error(const char *msg)
{
    printf("%s%s%s", RED, msg, RESET);
    return (1);
}

void log_event(simulation_t *sim, int philo_id, const char *msg)
{
    long long t = ms_since(sim->start_ms);
    pthread_mutex_lock(&sim->log_lock);
    if (get_running(sim) || ft_strcmp(msg, "died") == 0)
        printf("%s%lld%s %s%d%s %s\n", RED, t, RESET, GREEN, philo_id + 1, RESET, msg);
    pthread_mutex_unlock(&sim->log_lock);
}
