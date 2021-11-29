/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 18:11:55 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 17:34:25 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	find_argv(const char *str)
{
	long int	n;
	int			max_int;

	max_int = 2147483647;
	while (*str == ' ' || *str == '\f' || *str == '\n' || \
			*str == '\r' || *str == '\t' || *str == '\v')
		str++;
	n = 0;
	if (*str == '-')
		return (-1);
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + *str - '0';
		if (n > max_int)
			return (-1);
		str++;
	}
	if (*str != '\0' || n == 0)
		return (-1);
	return ((int)n);
}

static t_philo	*init_data(int argc, char **argv)
{
	t_philo	*tmp;

	tmp = (t_philo *)malloc(sizeof(t_philo));
	if (!tmp)
		ft_error("Error: Failed to malloc philo");
	tmp->num_philos = find_argv(argv[1]);
	tmp->num_forks = tmp->num_philos;
	tmp->t_die = find_argv(argv[2]);
	tmp->t_eat = find_argv(argv[3]);
	tmp->t_sleep = find_argv(argv[4]);
	if (tmp->num_philos < 1 || tmp->num_philos > 250 || tmp->t_die == -1 || \
		tmp->t_eat == -1 || tmp->t_sleep == -1)
		ft_error("Error: Wrong arguments");
	tmp->num_eat = -1;
	if (argc == 6)
	{
		tmp->num_eat = find_argv(argv[5]);
		if (tmp->num_eat == -1)
			ft_error("Error: Wrong arguments");
	}
	tmp->num_eat_count = 0;
	tmp->stop = 0;
	tmp->died = 0;
	return (tmp);
}

t_philo	*init_philo(int argc, char **argv)
{
	t_philo	*tmp;

	if (argc < 5 || argc > 6)
		ft_error("Error: Wrong number of arguments");
	tmp = init_data(argc, argv);
	tmp->pid = (int *)malloc(sizeof(int) * tmp->num_forks);
	if (!tmp->pid)
		ft_error("Error: malloc error (init pid)");
	sem_unlink("/block_print");
	sem_unlink("/block_forks");
	tmp->block_printf = sem_open("/block_print", O_CREAT, 0644, 1);
	tmp->block_fork = sem_open("/block_forks", O_CREAT, \
								0644, tmp->num_forks);
	if (tmp->block_printf <= 0 || tmp->block_fork <= 0)
		ft_error("Error: semaphore open error");
	return (tmp);
}
