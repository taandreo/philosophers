/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 15:40:15 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/07 15:44:32 by tairribe         ###   ########.fr       */
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

void	set_last_meal(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal = get_time();
	pthread_mutex_unlock(&philosopher->meal_lock);
}

t_bool	check_stop(t_philosopher *philosopher)
{
	t_bool	stop_flag;

	stop_flag = false;
	pthread_mutex_lock(&philosopher->data->stop_mutex);
	if (philosopher->data->stop)
		stop_flag = true;
	pthread_mutex_unlock(&philosopher->data->stop_mutex);
	return (stop_flag);
}

void	set_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = true;
	pthread_mutex_unlock(&data->stop_mutex);
}

t_bool	check_death(t_philosopher *philo)
{
	if (get_time() - philo->last_meal > philo->data->time_to_die)
	{
		set_stop(philo->data);
		print_status(philo, "died");
		return (true);
	}
	return (false);
}
