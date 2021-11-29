/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:00:55 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 19:35:19 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*check_monitor(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->stop)
	{
		usleep(100);
		if (find_time() - philo->t_meal > philo->t_die)
		{
			philo->died = 1;
			sem_wait(philo->block_printf);
			printf("%lld %d %s\n", \
					find_time() - philo->t_start, philo->index, "died");
			philo->stop = 1;
			break ;
		}
		if (philo->num_eat != -1 && philo->num_eat_count >= philo->num_eat)
		{
			philo->stop = 1;
			break ;
		}
	}
	if (philo->died)
		exit (1);
	else
		exit (0);
}

void	philo_start(t_philo *philo)
{
	if (pthread_create(&philo->check_monitor, \
			NULL, &check_monitor, philo))
		ft_error("Error: Failed to create the thread");
	if (philo->index % 2 == 1)
		usleep(1000);
	while (1)
	{
		philo_print(philo, "is thinking");
		sem_wait(philo->block_fork);
		philo_print(philo, "has taken a fork");
		sem_wait(philo->block_fork);
		philo_print(philo, "has taken a fork");
		philo_print(philo, "is eating");
		upgrade_sleep(philo->t_eat, philo);
		philo->t_meal = find_time();
		sem_post(philo->block_fork);
		sem_post(philo->block_fork);
		philo->num_eat_count += 1;
		philo_print(philo, "is sleeping");
		upgrade_sleep(philo->t_sleep, philo);
	}
	if (pthread_join(philo->check_monitor, NULL))
		ft_error("Error: Failed to join the thread");
}

void	exit_philo(t_philo **philo)
{
	t_philo	*tmp;
	int		i;
	int		status;

	tmp = *philo;
	i = 0;
	while (i < tmp->num_philos)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = -1;
			while (++i < tmp->num_philos)
				kill(tmp->pid[i], SIGKILL);
			break ;
		}
		i++;
	}
	sem_close(tmp->block_printf);
	sem_close(tmp->block_fork);
	sem_unlink("/block_print");
	sem_unlink("/block_forks");
	free(tmp->pid);
	free(tmp);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		i;

	philo = init_philo(argc, argv);
	i = -1;
	philo->t_start = find_time();
	while (++i < philo->num_philos)
	{
		philo->pid[i] = fork();
		if (philo->pid[i] == -1)
			ft_error("Error: fork mistake");
		if (philo->pid[i] == 0)
		{
			philo->index = i + 1;
			philo->t_meal = find_time();
			philo_start(philo);
		}
	}
	exit_philo(&philo);
	return (0);
}
