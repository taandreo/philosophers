/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 23:45:41 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/06 19:09:44 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

t_bool	check_stop(t_philosopher	*philosopher)
{
	t_bool stop_flag;

	stop_flag = false;
	pthread_mutex_lock(&philosopher->data->stop_mutex);
	if (philosopher->data->stop == true)
		stop_flag = true;
	pthread_mutex_unlock(&philosopher->data->stop_mutex);
	return stop_flag;
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *) arg;
	while (1)
	{
		if (check_stop(philosopher))
			return (NULL);
		if (philosopher->data->nb_philos == 1)
		{
			pthread_mutex_lock(philosopher->left_fork);
			print_status(philosopher, "has taken a fork");
			usleep(philosopher->data->time_to_die * 1000 + 1000);
			pthread_mutex_unlock(philosopher->left_fork);
			return(NULL);
		}
		eat(philosopher);
		if (check_stop(philosopher))
			return (NULL);
		snooze(philosopher);
		think(philosopher);
	}
	return (NULL);
}
