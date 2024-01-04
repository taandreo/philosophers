/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/04 03:30:37 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philosopher **init_philosophers(t_data *data)
{
	t_philosopher	**philosophers;
	int 			i;

	philosophers = ft_calloc(data->number_of_philosophers + 1, sizeof(t_philosopher *));
	i = 0;
	while (i < data->number_of_philosophers)
	{
		philosophers[i] = ft_calloc(1, sizeof(t_philosopher));
		philosophers[i]->id = i + 1;
		philosophers[i]->data = data;
		philosophers[i]->left_fork = ft_calloc(1, sizeof(pthread_mutex_t));
		pthread_mutex_init(philosophers[i]->left_fork, NULL);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (i > 0)
			philosophers[i - 1]->right_fork = philosophers[i]->left_fork;
		else
			philosophers[data->number_of_philosophers - 1]->right_fork = philosophers[i]->left_fork;
		i++;
	}
	return (philosophers);
}

/* Start the dinner, by initializating a thread for each philosopher.
The philosophers with even id, are started microseconds after the last odd
philosopher, so we can initializate half of the philosophers dinner */
void	start_dinner(t_philosopher **philosophers, t_data *data)
{
	int i;

	i = 0;
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();
	while (i < data->number_of_philosophers)
	{
		if (philosophers[i]->id % 2 == 0)
			usleep(300);
		philosophers[i]->last_meal = data->start_time;
		pthread_create(&philosophers[i]->thread, NULL, &routine, philosophers[i]);
		// pthread_detach(philosophers[i]->thread);
		i++;
	}
}

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
		while(i < data->number_of_philosophers)
		{
			if (get_time() - philosophers[i]->last_meal > data->time_to_die)
			{
				print_status(philosophers[i], "died");
				data->stop = true;
				return ;
			}
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
	if (data.number_of_philosophers == 0)
		return (0);
	philosophers = init_philosophers(&data);
	start_dinner(philosophers, &data);
	death_routine(philosophers);
	wait_philosophers(philosophers);
	free_philosophers(philosophers);
}
