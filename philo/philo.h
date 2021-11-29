/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 16:09:19 by cdarrell          #+#    #+#             */
/*   Updated: 2021/11/07 18:33:54 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num_philos;
	int				num_forks;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				num_eat;
	int				stop;
	pthread_mutex_t	*mutex_fork;
	pthread_mutex_t	mutex_printf;
	long long int	t_start;
	pthread_t		check_monitor;
}t_data;

typedef struct s_philo
{
	int				index;
	int				num_eat_count;
	long long int	t_meal;
	t_data			*data;
	pthread_mutex_t	*lf;
	pthread_mutex_t	*rf;
	pthread_t		pth_t;
}t_philo;

//init
int			init_philos(t_philo **philos, int argc, char **argv);

//unity
int			ft_error(char *str);
long long	find_time(void);
void		upgrade_sleep(long long time, t_data *data);

#endif
