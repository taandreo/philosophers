/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 23:45:41 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/06 21:12:36 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(t_philosopher *philosopher, char *status)
{
	pthread_mutex_lock(&philosopher->data->print);
	printf("%ld %d %s\n", get_timestamp(philosopher->data->start_time),
		philosopher->id, status);
	pthread_mutex_unlock(&philosopher->data->print);
}

void	get_forks(t_philosopher	*philosopher)
{
	if (philosopher->id != philosopher->data->nb_philos)
	{
		pthread_mutex_lock(philosopher->left_fork);
		print_status(philosopher, "has taken a left fork");
		pthread_mutex_lock(philosopher->right_fork);
		print_status(philosopher, "has taken a fork");
	} else {
		pthread_mutex_lock(philosopher->right_fork);
		print_status(philosopher, "has taken a right fork");
		pthread_mutex_lock(philosopher->left_fork);
		print_status(philosopher, "has taken a fork");
	}
}

void	eat(t_philosopher	*philosopher)
{
	get_forks(philosopher);
	print_status(philosopher, "is eating");
	philosopher->last_meal = get_time();
	usleep(philosopher->data->time_to_eat * 1000);
	philosopher->meals++;
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	snooze(t_philosopher	*philosopher)
{
	print_status(philosopher, "is sleeping");
	usleep(philosopher->data->time_to_sleep * 1000);
}

void	think(t_philosopher	*philosopher)
{
	print_status(philosopher, "is thinking");
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *) arg;
	while (1)
	{
		if (philosopher->data->nb_philos == 1)
		{
			pthread_mutex_lock(philosopher->left_fork);
			print_status(philosopher, "has taken a fork");
			usleep(philosopher->data->time_to_die * 1000 + 1000);
			pthread_mutex_unlock(philosopher->left_fork);
			return(NULL);
		}
		eat(philosopher);
		if (philosopher->data->stop)
			return (NULL);
		snooze(philosopher);
		if (philosopher->data->stop)
			return (NULL);
		think(philosopher);
	}
	return (NULL);
}
