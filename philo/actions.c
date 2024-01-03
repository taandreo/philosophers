/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 23:45:41 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/03 00:10:42 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philosopher	*philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	print_status(philosopher, "has taken a fork");
	pthread_mutex_lock(philosopher->right_fork);
	print_status(philosopher, "has taken a fork");
	print_status(philosopher, "is eating");
	philosopher->last_meal = get_time();
	usleep(philosopher->data->time_to_eat * 1000); // 1 milliseconds = 1000 microseconds
	philosopher->meals++;
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	snooze(t_philosopher	*philosopher)
{
	print_status(philosopher, "is sleeping");
	usleep(philosopher->data->time_to_sleep * 1000); // 1 milliseconds = 1000 microseconds
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
		eat(philosopher);
		snooze(philosopher);
		think(philosopher);
	}
	return (NULL);
}
