/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/07 17:51:02 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

char	*check_args(int argc, char **argv)
{
	int		i;
	long	nb;

	if (argc > 6)
		return (ft_strdup("Error: too many arguments"));
	if (argc < 5)
		return (ft_strdup("Error: too few arguments"));
	i = 1;
	while (i < argc)
	{
		nb = ft_strtol(argv[i]);
		if (!ft_is_number(argv[i]) || nb < 0 || nb > INT_MAX)
			return (ft_strdup("Error: arguments must be a integer positive"));
		i++;
	}
	return (NULL);
}

void	read_args(int argc, char **argv, t_data *data)
{
	data->nb_philos = ft_strtol(argv[1]);
	data->time_to_die = ft_strtol(argv[2]);
	data->time_to_eat = ft_strtol(argv[3]);
	data->time_to_sleep = ft_strtol(argv[4]);
	if (argc == 6)
		data->total_meals = ft_strtol(argv[5]);
	else
		data->total_meals = -1;
	data->stop = false;
}

t_philosopher	*init_philosopher(t_data *data, int id)
{
	t_philosopher	*philo;

	philo = ft_calloc(1, sizeof(t_philosopher));
	philo->id = id;
	philo->data = data;
	philo->left_fork = ft_calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->left_fork, NULL);
	pthread_mutex_init(&philo->meal_lock, NULL);
	return (philo);
}

t_philosopher	**init_philosophers(t_data *data)
{
	t_philosopher	**philos;
	int				i;

	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
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

void	free_philosophers(t_philosopher **philosophers, t_data *data)
{
	int	i;

	i = 0;
	while (philosophers[i])
	{
		pthread_mutex_destroy(philosophers[i]->left_fork);
		pthread_mutex_destroy(&philosophers[i]->meal_lock);
		free(philosophers[i]->left_fork);
		free(philosophers[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->stop_mutex);
	free(philosophers);
}
