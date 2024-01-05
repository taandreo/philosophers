/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/05 02:26:22 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	wait_philosophers(t_philosopher **philosophers)
{
	int	i;

	i = 0;
	while (philosophers[i])
	{
		pthread_join(philosophers[i]->thread, NULL);
		i++;
	}
}

t_bool	check_death(t_philosopher *philo)
{
	if (get_time() - philo->last_meal > philo->data->time_to_die)
	{
		print_status(philo, "died");
		philo->data->stop = true;
		return (true);
	}
	return (false);
}

void	death_routine(t_philosopher	**philosophers)
{
	int				i;
	t_data			*data;
	t_bool			all_philos_are_full;

	data = philosophers[0]->data;
	while (1)
	{
		i = 0;
		all_philos_are_full = true;
		while (i < data->nb_philos)
		{
			if (check_death(philosophers[i]))
				return ;
			if (philosophers[i]->meals < data->total_meals)
				all_philos_are_full = false;
			i++;
		}
		if (data->total_meals != -1 && all_philos_are_full)
		{
			data->stop = true;
			return ;
		}
		usleep(1000);
	}
}

void	start_dinner(t_philosopher **philos, t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();
	while (i < data->nb_philos)
	{
		if (philos[i]->id % 2 == 0)
			usleep(300);
		philos[i]->last_meal = data->start_time;
		pthread_create(&philos[i]->thread, NULL, &routine, philos[i]);
		i++;
	}
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
	death_routine(philosophers);
	wait_philosophers(philosophers);
	free_philosophers(philosophers);
}
