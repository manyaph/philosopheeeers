#include "philo.h"
long long get_time(void)
{
	struct timeval	time;

	gettimeofday (&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
int is_dead (t_philo *philo)
{
    pthread_mutex_lock(&philo->time_mutex);
    if (get_time() - philo->current_time > philo->time_to_die)
        return (1);
    pthread_mutex_unlock(&philo->time_mutex);
    return(0);
}


// int end_simulation(t_rules *rules)
// {
//     int i = 0;
//     while (!is_dead(&(rules->philo[i])) && rules->philo[i].eat_count < rules->eat_number && i < rules->number_of_philos)
//         i++;
//     return (i);
//     printf("ay sra hamar prts\n");
// }

int	end_simulation(t_rules *rules)
{
	int	i;
	int	philos_finished;

	i = 0;
	philos_finished = 0;
	while (is_dead(&rules->philo[i]) == 0 && i < rules->number_of_philos)
	{
		if (rules->philo[i].eat_count >= rules->eat_number)
            philos_finished++;
        if (philos_finished == rules->number_of_philos)
		    return (1);
		i++;
	}
	return (0);
}

void checking (t_rules *rules)
{
    int i;

    i = 0;
    while (1)
    {
        if (i == rules->number_of_philos)
            i = 0;
        if (is_dead(&rules->philo[i]))
        {
            pthread_mutex_lock(rules->prompt_mutex);
			pthread_mutex_unlock(&rules->philo[i].time_mutex);
            printf("%lld %d id dead\n", get_time() - rules->philo[i].start, rules->philo[i].philo);
            printf("joxovurd mard a mahacel\n");
            break ;
        }

        else if (rules->eat_number)
            if (end_simulation(rules))
                break ;

        i++;
    }
}


void rules_init (int argc, char **argv, t_rules *rules)
{
    printf("ste enq\n");
    rules->number_of_philos = atoi(argv[1]);
    rules->time_to_die = atoi(argv[2]);
    rules->time_to_eat = atoi(argv[3]);
    rules->time_to_sleep = atoi(argv[4]);
    printf("hasanq ste\n");
    if (argc == 6)
    {
        rules->eat_number = atoi(argv[5]);
        if (rules->eat_number <= 0)
            exit(write(1, "aaa\n", 4));
    }
    else    
        rules->eat_number = 0;
    if (rules->number_of_philos <= 0 ||  rules->time_to_die <= 0 ||  rules->time_to_eat <= 0 ||  rules->time_to_sleep <= 0)
        exit(write(1, "aaa\n", 4));
    printf("rules prts\n");    
}

void mutex_init (t_rules *rules)
{
    int i = 0;
    rules->fork_mutex = malloc(sizeof(mutex) * rules->number_of_philos);
    rules->prompt_mutex = malloc(sizeof(mutex));
    if (!rules->fork_mutex || !rules->prompt_mutex)
        exit(write(1, "aaa\n", 4));
    printf("hasanq ste\n");    
    if (pthread_mutex_init(rules->prompt_mutex, NULL) != 0)
        exit(write(1, "aaa\n", 4));
    while (i < rules->number_of_philos)
    {
        if (pthread_mutex_init(&rules->fork_mutex[i], NULL) != 0)
            exit(write(1, "aaa\n", 4));
        i++;
    }
    printf("mutex prts\n");    
}

void philo_init (t_rules *rules)
{
    int i = 0;
    while (i < rules->number_of_philos)
    {
        rules->philo[i].philo = i;
        rules->philo[i].eat_count = 0;
        rules->philo[i].current_time = get_time();
        rules->philo[i].start = get_time();
        rules->philo[i].time_to_die = rules->time_to_die;
        rules->philo[i].time_to_eat = rules->time_to_eat;
        rules->philo[i].time_to_sleep = rules->time_to_sleep;
        rules->philo[i].prompt_mutex = rules->prompt_mutex;
        rules->philo[i].left_fork_mutex = &(rules->fork_mutex[i]);
        if (i != rules->number_of_philos - 1)
            rules->philo[i].right_fork_mutex = &(rules->fork_mutex[i + 1]);
        else 
            rules->philo[i].right_fork_mutex = &(rules->fork_mutex[0]);
        printf("hasanq ste %d\n", i);    
        if (pthread_mutex_init(&(rules->philo[i].time_mutex), NULL) != 0)
            exit(write(1, "aaa\n", 4));

        i++;
    }
    printf("philo prts\n");    
}

void	ft_usleep(unsigned long time)
{
	unsigned long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
		usleep(100);
}

void	ft_print_message(t_philo *philo, char *str)
{
	pthread_mutex_lock(philo->prompt_mutex);
	printf("%llu %d %s\n", get_time() - philo->start,
		philo->philo, str);
	pthread_mutex_unlock(philo->prompt_mutex);
}

void *philoing (void *ph)
{
    t_philo *philo;

    philo = (t_philo *)ph;
    if (philo->philo %2 == 0)
        ft_usleep(philo->time_to_eat);
    while (1)
    {
		pthread_mutex_lock(philo->left_fork_mutex);
		ft_print_message(philo, "has taken left fork");
		pthread_mutex_lock(philo->right_fork_mutex);
		ft_print_message(philo, "has taken right fork");
		pthread_mutex_lock(&philo->time_mutex);
		philo->eat_count++;
		philo->current_time = get_time();
		pthread_mutex_unlock(&philo->time_mutex);
		ft_print_message(philo, "is eating");
		ft_usleep(philo->time_to_eat);
		pthread_mutex_unlock(philo->left_fork_mutex);
		pthread_mutex_unlock(philo->right_fork_mutex);
		ft_print_message(philo, "is sleeping");
		ft_usleep (philo->time_to_sleep);
		ft_print_message(philo, "is thinking");
    }
    return (NULL);
}


void philo_create (t_rules *rules)
{
    int i = 0;

    while (i < rules->number_of_philos)
    {
        if (pthread_create(&rules->philo[i].th, NULL, philoing, (void*)&rules->philo[i]) != 0)
             exit(write(1, "aaa\n", 4));
        i++;
    }
    checking(rules);
}



int main (int argc, char **argv)
{
    t_rules *rules;
    rules = malloc(sizeof(t_rules));
    rules_init (argc, argv, rules);
    mutex_init(rules);
    rules->philo = malloc(sizeof(t_philo) * rules->number_of_philos);
    philo_init(rules);
    philo_create(rules);
    return (0);
}