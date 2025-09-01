#include "philosophers.h"

int errorf(const char *msg)
{
    printf("%s%s%s", RED, msg, RESET);
    return (1);
}

void log_event(sim_t *sim, int actor_id, const char *msg)
{
    long long t = ms_since(sim->start_ms);
    int running;
    pthread_mutex_lock(&sim->guard);
    running = sim->running;
    pthread_mutex_unlock(&sim->guard);
    pthread_mutex_lock(&sim->log_lock);
    if (running)
        printf("%s%lld%s %s%d%s %s\n", RED, t, RESET, GREEN, actor_id + 1, RESET, msg);
    pthread_mutex_unlock(&sim->log_lock);
}
