/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/05 02:31:53 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philosopher	*init_philosopher(t_data *data, int id)
{
	t_philosopher	*philo;

	philo = ft_calloc(1, sizeof(t_philosopher));
	philo->id = id;
	philo->data = data;
	philo->left_fork = ft_calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->left_fork, NULL);
	return (philo);
}

t_philosopher	**init_philosophers(t_data *data)
{
	t_philosopher	**philos;
	int				i;

	philos = ft_calloc(data->nb_philos + 1, sizeof(t_philosopher *));
	i = 0;
	while (i < data->nb_philos)
	{
		philos[i] = init_philosopher(data, i + 1);
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (i > 0)
			philos[i - 1]->right_fork = philos[i]->left_fork;
		else
			philos[data->nb_philos - 1]->right_fork = philos[i]->left_fork;
		i++;
	}
	return (philos);
}

void	free_philosophers(t_philosopher **philosophers)
{
	int	i;

	i = 0;
	while (philosophers[i])
	{
		pthread_mutex_destroy(philosophers[i]->left_fork);
		free(philosophers[i]->left_fork);
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
}
