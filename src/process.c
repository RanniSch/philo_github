/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschlott <rschlott@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 19:36:55 by rschlott          #+#    #+#             */
/*   Updated: 2023/01/04 21:12:21 by rschlott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/philo.h"

/* assigns forks to each philosopher;
uneven number philo (% 2 != 0) takes own fork first and then fork of neighbour;
even number philo takes first fork of neighbour and then own fork;
%philo->table->nb_philos equals fork of neighbour (last philo gets from first) */
static void assign_forks(t_philo *philo)
{
    if (philo->id % 2 != 0)
    {
        philo->fork[0] = philo->id;
        philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
    }
    else
    {
        philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
        philo->fork[1] = philo->id;
    }
    //printf("philo: %d fork1: %d fork2: %d\n", philo->id, philo->fork[0], philo->fork[1]);
}

/* creates/initializes the philosphers' structs and returns array
of philosophers which is put into the table struct;
With this the forks get also saved in there, as to each philo belongs a fork!
pthread_mutex_init:initializes mutex (here eating time for each philosopher), 
returns 0 if successful;
times_ate set to 0 for beginning of simulation */
static t_philo	**init_philosophers(t_table *table)
{
    t_philo **philos;
    unsigned int i;

    i = 0;
    philos = malloc(sizeof(t_philo) * table->nb_philos);
    if (!philos)
        return (error_null(STR_ERR_MALLOC, NULL, 0));
    while (i < table->nb_philos)
    {
        philos[i] = malloc(sizeof(t_philo));
        if (!philos[i])
            return (error_null(STR_ERR_MALLOC, NULL, 0));
        if (pthread_mutex_init(&philos[i]->eat_time_lock, NULL) != 0)
            return (error_null(STR_ERR_MUTEX, NULL, 0));
        philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
        assign_forks(philos[i]);
        i++;
    }
    return (philos);
}

/* initialize mutexes (locker) for each fork (mutex = the fork);
return (is an array of fork-mutexes) */
static pthread_mutex_t *init_forks(t_table *table)
{
    pthread_mutex_t *lock_forks;
    unsigned int    i;

    lock_forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
    if (!lock_forks)
        return (error_null(STR_ERR_MUTEX, NULL, 0));
    i = 0;
    while (i < table->nb_philos)
    {
        if (pthread_mutex_init(&lock_forks[i], 0) != 0)
            return (error_null(STR_ERR_MUTEX, NULL, 0));
        i++;
    }
    return (lock_forks);
}

/* initialization of mutexes for forks, writing and simulation itself
via pthread_mutex_init */
static bool init_global_mutexes(t_table *table)
{
    table->fork_locks = init_forks(table);
    if (!table->fork_locks)
        return (false);
    if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0) // tauschen mit do_lock??? 
	{
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	}
	if (pthread_mutex_init(&table->do_lock, 0) != 0)
	{
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	}
    return (true);
}

/* puts user input values to variables;
then calls other functions;
sim_stop set to false as before start of simulation */
t_table *init_process (int argc, char **argv)
{
    t_table *table;
    int i;

    i = 1;
    table = malloc(sizeof(t_table));
    if (!table)
        return (error_null(STR_ERR_MALLOC, NULL, 0));
    table->nb_philos = ft_atoi(argv[i]);
	i++;
	table->time_to_die = ft_atoi(argv[i]);
	i++;
	table->time_to_eat = ft_atoi(argv[i]);
	i++;
	table->time_to_sleep = ft_atoi(argv[i]);
	i++;
	table->must_eat_count = -1;
    if (argc == 6)
		table->must_eat_count = ft_atoi(argv[i]);
    //printf("philos: %d, time to die %ld, time to eat %ld, time to sleep %ld, must eat %d.\n", table->nb_philos, table->time_to_die, table->time_to_eat, table->time_to_sleep, table->must_eat_count);
    table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
    if (!init_global_mutexes(table))
        return (NULL);
    table->sim_stop = false;
    return (table);
}
