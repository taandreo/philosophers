/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 17:27:43 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/02 20:50:38 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_timestamp(long start_time)
{
	long	current_time;

	current_time = get_time();
	return (current_time - start_time);
}

void	print_status(t_philosopher *philosopher, char *status)
{
	pthread_mutex_lock(&philosopher->data->print);
	printf("%ld %d %s\n", get_timestamp(philosopher->data->start_time),
		philosopher->id, status);
	pthread_mutex_unlock(&philosopher->data->print);
}