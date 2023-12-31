/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 23:45:41 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/07 15:46:31 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philosopher *philosopher)
{
	if (philosopher->id != philosopher->data->nb_philos)
	{
		pthread_mutex_lock(philosopher->right_fork);
		pthread_mutex_lock(philosopher->left_fork);
	}
	else
	{
		pthread_mutex_lock(philosopher->left_fork);
		pthread_mutex_lock(philosopher->right_fork);
	}
	if (check_stop(philosopher))
	{
		pthread_mutex_unlock(philosopher->right_fork);
		pthread_mutex_unlock(philosopher->left_fork);
		return ;
	}
	print_status(philosopher, "has taken a fork");
	print_status(philosopher, "has taken a fork");
	print_status(philosopher, "is eating");
	set_last_meal(philosopher);
	usleep(philosopher->data->time_to_eat * 1000);
	philosopher->meals++;
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

void	snooze(t_philosopher	*philosopher)
{
	print_status(philosopher, "is sleeping");
	usleep(philosopher->data->time_to_sleep * 1000);
}

void	think(t_philosopher	*philosopher)
{
	print_status(philosopher, "is thinking");
	usleep(500);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *) arg;
	if (philosopher->id % 2 == 0)
		usleep(1000);
	while (!check_stop(philosopher))
	{
		if (philosopher->data->nb_philos == 1)
		{
			pthread_mutex_lock(philosopher->left_fork);
			print_status(philosopher, "has taken a fork");
			usleep(philosopher->data->time_to_die * 1000 + 1000);
			pthread_mutex_unlock(philosopher->left_fork);
			return (NULL);
		}
		eat(philosopher);
		if (check_stop(philosopher))
			return (NULL);
		snooze(philosopher);
		think(philosopher);
	}
	return (NULL);
}
