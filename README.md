# Philosophers (Refactored)

Modern, modular C implementation of the Dining Philosophers simulation.

## Usage

- Build: `make`
- Run: `./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_meals]`
- Help: `./philo --help`

Arguments:
- `number_of_philos`: integer >= 1
- `time_to_die`: ms > 0
- `time_to_eat`: ms > 0
- `time_to_sleep`: ms > 0
- `max_meals` (optional): integer >= 0, omit for unlimited

## Layout

- `include/philosophers.h`: Public types and prototypes
- `src/args.c`: Parsing, validation, and initialization
- `src/sim.c`: Threads, monitor loop, and teardown
- `src/time.c`: Time helpers (millisecond precision)
- `src/log.c`: Logging and fatal error handling

## Design Notes

- Deadlock prevention by ordered fork locking (min→max index)
- Single philosopher handled gracefully (take one fork, wait t_die, release)
- Thread creation failures cleanly unwind (joins only created threads)
- Shared state (`last_meal_ms`) guarded by a mutex
- Consistent, single-line logging: `<time_ms> <philo_id> <event>`

