/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 18:11:55 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 18:34:28 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (*str != '\0')
		return (-1);
	return ((int)n);
}

static int	init_data(t_data **data, int argc, char **argv)
{
	t_data	*tmp;

	tmp = (t_data *)malloc(sizeof(t_data));
	if (!tmp)
		return (ft_error("Error: Failed to malloc data"));
	tmp->num_philos = find_argv(argv[1]);
	tmp->num_forks = tmp->num_philos;
	tmp->t_die = find_argv(argv[2]);
	tmp->t_eat = find_argv(argv[3]);
	tmp->t_sleep = find_argv(argv[4]);
	if (tmp->num_philos < 1 || tmp->num_philos > 250 || tmp->t_die == -1 || \
		tmp->t_eat == -1 || tmp->t_sleep == -1)
		return (ft_error("Error: Wrong arguments"));
	tmp->num_eat = -1;
	if (argc == 6)
	{
		tmp->num_eat = find_argv(argv[5]);
		if (tmp->num_eat == -1)
			return (ft_error("Error: Wrong arguments"));
	}
	tmp->stop = 0;
	*data = tmp;
	return (0);
}

static int	init_mutex_fork(t_data *data)
{
	int				i;

	data->mutex_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) \
					* data->num_forks);
	if (!data->mutex_fork)
		return (ft_error("Error: Failed to malloc forks"));
	i = -1;
	while (++i < data->num_forks)
	{
		if (pthread_mutex_init(data->mutex_fork + i, NULL))
			return (ft_error("Error: pthread_mutex_fork"));
	}
	return (0);
}

static void	init_philo(t_philo *philo, int i, \
						t_data *data, pthread_mutex_t *forks)
{
	philo->index = i + 1;
	philo->num_eat_count = 0;
	philo->t_meal = 0;
	philo->data = data;
	if (i == 0)
	{
		philo->lf = forks + data->num_philos - 1;
		philo->rf = forks + i;
	}
	else
	{
		philo->lf = forks + i - 1;
		philo->rf = forks + i;
	}
}

int	init_philos(t_philo **philos, int argc, char **argv)
{
	t_data	*data;
	t_philo	*tmp;
	int		i;

	if (init_data(&data, argc, argv) == -1)
		return (-1);
	if (pthread_mutex_init(&data->mutex_printf, NULL))
		return (ft_error("Error: pthread_mutex_print"));
	if (init_mutex_fork(data) == -1)
		return (-1);
	tmp = (t_philo *)malloc(sizeof(t_philo) * data->num_philos);
	if (!tmp)
		return (ft_error("Error: Failed to malloc philos"));
	i = -1;
	while (++i < data->num_philos)
		init_philo(tmp + i, i, data, data->mutex_fork);
	*philos = tmp;
	return (0);
}
