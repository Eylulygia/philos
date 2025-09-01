#include "philosophers.h"

long long now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

long long ms_since(long long past)
{
    return now_ms() - past;
}

void sleep_for(long long ms, sim_t *sim)
{
    long long start = now_ms();
    while (1)
    {
        int running;
        pthread_mutex_lock(&sim->guard);
        running = sim->running;
        pthread_mutex_unlock(&sim->guard);
        if (!running)
            break;
        if (ms_since(start) >= ms)
            break;
        usleep(50);
    }
}
