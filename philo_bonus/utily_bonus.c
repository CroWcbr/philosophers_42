/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utily_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:12:37 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 18:23:31 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_error(char *str)
{
	int	i;

	i = 0;
	while (*(str + i))
		i++;
	write(2, str, i);
	write(2, "\n", 1);
	exit(0);
}

long long	find_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	upgrade_sleep(long long time, t_philo *philo)
{
	long long	t;

	t = find_time();
	while (!philo->stop)
	{
		if (find_time() - t >= time)
			break ;
		usleep(500);
	}
}

void	philo_print(t_philo *philo, char *str)
{
	sem_wait(philo->block_printf);
	if (!philo->stop)
		printf("%lld %d %s\n", find_time() - philo->t_start, philo->index, str);
	sem_post(philo->block_printf);
}
