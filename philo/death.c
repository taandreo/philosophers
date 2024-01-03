/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 20:50:03 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/02 23:01:04 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	death_routine(t_philosopher	**philosophers)
{
	// t_philosopher	**philosophers;
	int				i;
	t_data			*data;
	long			now;
	
	// philosophers = (t_philosopher **) arg;		
	data = philosophers[0]->data;
	while (1)
	{
		i = 0;
		while(i < data->number_of_philosophers)
		{
			now = get_time();
			if (now - philosophers[i]->last_meal > data->time_to_die)
			{
				print_status(philosophers[i], "died");
				return ;
			}
		}
		usleep(1000);				
	}
}