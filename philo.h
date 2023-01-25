#ifndef PHILO_H
#define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef pthread_mutex_t mutex;

typedef struct s_philo
{
    int philo;
    int eat_count;
    long long current_time;
    long long start;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    pthread_t th;
    mutex *left_fork_mutex;
    mutex *right_fork_mutex;
    mutex *prompt_mutex;
    mutex time_mutex;
}   t_philo;

typedef struct s_rules
{
    int number_of_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int eat_number;
    mutex *fork_mutex;
    mutex *prompt_mutex;
    t_philo *philo;
}   t_rules;


#endif