#include "philosophers.h"

int	ft_atoi(char *str)
{
	unsigned long long	nb;
	int					sign;
	int					i;

	nb = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (sign * nb);
}
int	exceeds_int_max(const char *s)
{
	long long	val;
	int			i;

	if (!s || !*s)
		return (1);
	val = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		val = val * 10 + (s[i] - '0');
		if (val > INT_MAX)
			return (1);
		i++;
	}
	return (0);
}

int	print_error(const char *msg)
{
	printf("%s%s%s", RED, msg, RESET);
	return (1);
}

void	log_event(t_simulation *sim, int philo_id, const char *msg)
{
	long long	t;

	t = ms_since(sim->start_ms);
	pthread_mutex_lock(&sim->log_lock);
	if (get_running(sim) || ft_strcmp(msg, "died") == 0)
		printf("%s%lld%s %s%d%s %s\n", RED, t, RESET, GREEN, philo_id + 1,
			RESET, msg);
	pthread_mutex_unlock(&sim->log_lock);
	//usleep(50);
}
