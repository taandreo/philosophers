/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/01 23:37:09 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_data ;

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	s_data			*data;
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;
}	t_philosopher;

void	read_args(int argc, char **argv, t_data *data)
{
	data->number_of_philosophers = ft_strtol(argv[1]);
	data->time_to_die = ft_strtol(argv[2]);
	data->time_to_eat = ft_strtol(argv[3]);
	data->time_to_sleep = ft_strtol(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_strtol(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = -1;
}

void	eat(t_philosopher	*philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	pthread_mutex_lock(philosopher->right_fork);
	printf("%d is eating\n", philosopher->id);
	usleep(philosopher->data->time_to_eat * 1000);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	sleep(t_philosopher	*philosopher)
{
	printf("%d is sleeping\n", philosopher->id);
	usleep(philosopher->data->time_to_sleep * 1000);
}

void	think(t_philosopher	*philosopher)
{
	printf("%d is thinking\n", philosopher->id);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;
	philosopher = (t_philosopher *) arg;
	eat(philosopher);
	sleep(philosopher);
	think(philosopher);
	return (NULL);
}

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
		pthread_mutex_init(philosophers[i]->left_fork, NULL);
		if (i > 0)
			philosophers[i - 1]->right_fork = philosophers[i]->left_fork
		else
			philosophers[data->number_of_philosophers - 1]->right_fork = philosophers[i]->left_fork;
		pthread_create(&philosophers[i]->thread, NULL, &routine, philosophers[i]);
		i++;
	}
	return (philosophers);
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

char	*check_args(int argc, char **argv)
{
	int		i;
	long	nb;

	if (argc > 6)
		return(ft_strdup("Error: too many arguments"));
	if (argc < 5)
		return(ft_strdup("Error: too few arguments"));
	i = 1;
	while (i < argc)
	{
		nb = ft_strtol(argv[i]);
		if (!ft_is_number(argv[i]) || nb < 0 || nb > INT_MAX)
			return(ft_strdup("Error: arguments must be integer positive numbers"));
		i++;
	}
	return (NULL);
}

void	free_philosophers(t_philosopher **philosophers)
{
	int	i;

	i = 0;
	while (philosophers[i])
	{
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
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
	philosophers = init_philosophers(&data);
	wait_philosophers(philosophers);
	free_philosophers(philosophers);
}
	
	