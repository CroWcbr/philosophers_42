/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:00:55 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 19:28:07 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->mutex_printf);
	if (!philo->data->stop)
		printf("%lld %d %s\n", \
			find_time() - philo->data->t_start, philo->index, str);
	pthread_mutex_unlock(&philo->data->mutex_printf);
}

void	*start(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->data->stop)
	{
		philo_print(philo, "is thinking");
		pthread_mutex_lock(philo->lf);
		philo_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->rf);
		philo_print(philo, "has taken a fork");
		philo_print(philo, "is eating");
		upgrade_sleep(philo->data->t_eat, philo->data);
		philo->t_meal = find_time();
		pthread_mutex_unlock(philo->rf);
		pthread_mutex_unlock(philo->lf);
		if (!philo->data->stop)
			philo->num_eat_count += 1;
		philo_print(philo, "is sleeping");
		upgrade_sleep(philo->data->t_sleep, philo->data);
	}
	return (0);
}

void	*check_monitor(void *args)
{
	t_philo	*philos;
	int		i;
	int		flag_all_eat;

	philos = (t_philo *)args;
	while (!philos->data->stop)
	{
		i = -1;
		flag_all_eat = 0;
		while (++i < philos->data->num_philos)
		{
			if (find_time() - (philos + i)->t_meal > philos->data->t_die)
			{
				philo_print(philos + i, "died");
				philos->data->stop = 1;
				break ;
			}
			if (philos->data->num_eat != -1 && \
				(philos + i)->num_eat_count >= philos->data->num_eat)
				flag_all_eat++;
		}
		if (flag_all_eat == philos->data->num_philos)
			philos->data->stop = 1;
	}
	return (0);
}

void	philo_start(t_philo *philos)
{
	int	i;

	i = -1;
	philos->data->t_start = find_time();
	while (++i < philos->data->num_philos)
	{	
		(philos + i)->t_meal = find_time();
		if (pthread_create(&(philos + i)->pth_t, NULL, &start, philos + i))
			ft_error("Error: Failed to create the thread");
		pthread_detach((philos + i)->pth_t);
		usleep(50);
	}
	if (pthread_create(&philos->data->check_monitor, \
						NULL, &check_monitor, philos))
		ft_error("Error: Failed to create the thread");
	if (pthread_join(philos->data->check_monitor, NULL))
		ft_error("Error: Failed to join the thread");
	i = -1;
	while (++i < philos->data->num_forks)
		pthread_mutex_destroy(&philos->data->mutex_fork[i]);
	pthread_mutex_destroy(&philos->data->mutex_printf);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (ft_error("Error: Wrong number of arguments"));
	if (init_philos(&philos, argc, argv) == -1)
		return (0);
	philo_start(philos);
	return (0);
}
