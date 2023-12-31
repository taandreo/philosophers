/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/07 18:57:40 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	wait_philosophers(t_philosopher **philosophers)
{
	int	i;

	i = 0;
	pthread_join(philosophers[i]->data->death_thread, NULL);
	while (philosophers[i])
	{
		pthread_join(philosophers[i]->thread, NULL);
		i++;
	}
}

void	*death_routine(void *arg)
{
	int				i;
	t_data			*data;
	t_bool			all_philos_are_full;
	t_philosopher	**philosophers;

	philosophers = (t_philosopher **) arg;
	data = philosophers[0]->data;
	while (1)
	{
		i = -1;
		all_philos_are_full = true;
		while (++i < data->nb_philos)
		{
			if (check_death(philosophers[i]))
				return (NULL);
			if (philosophers[i]->meals < data->total_meals)
				all_philos_are_full = false;
		}
		if (data->total_meals != -1 && all_philos_are_full)
		{
			set_stop(data);
			return (NULL);
		}
		usleep(4777);
	}
}

void	start_dinner(t_philosopher **philos, t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time();
	while (i < data->nb_philos)
	{
		philos[i]->last_meal = data->start_time;
		pthread_create(&philos[i]->thread, NULL, &routine, philos[i]);
		i++;
	}
	pthread_create(&data->death_thread, NULL, &death_routine, philos);
}

int	main(int argc, char **argv)
{
	char			*err;
	t_data			data;
	t_philosopher	**philosophers;

	err = check_args(argc, argv);
	if (err != NULL)
	{
		error(err);
		free(err);
		return (1);
	}
	read_args(argc, argv, &data);
	if (data.nb_philos == 0)
		return (0);
	philosophers = init_philosophers(&data);
	start_dinner(philosophers, &data);
	wait_philosophers(philosophers);
	free_philosophers(philosophers, &data);
}
