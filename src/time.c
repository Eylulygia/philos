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

void sleep_for(long long ms, simulation_t *sim)
{
    long long start = now_ms();
    while (get_running(sim))
    {
        if (ms_since(start) >= ms)
            break;
        usleep(50);
    }
}
