/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:37:25 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/02 23:24:20 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		pthread_detach(philosophers[i]->thread);
		i++;
	}
}

// void	wait_philosophers(t_philosopher **philosophers)
// {
// 	int	i;

// 	i = 0;
// 	while (philosophers[i])
// 	{
// 		pthread_join(philosophers[i]->thread, NULL);
// 		i++;
// 	}
// }

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
		pthread_mutex_destroy(philosophers[i]->left_fork);
		free(philosophers[i]->left_fork);
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
}

void	wait_philosophers(t_philosopher	**philosophers)
{
	// t_philosopher	**philosophers;
	int				i;
	t_data			*data;
	long			now;
	t_bool			full;
	
	// philosophers = (t_philosopher **) arg;		
	data = philosophers[0]->data;
	while (1)
	{
		i = 0;
		full = true;
		while(i < data->number_of_philosophers)
		{
			now = get_time();
			if (now - philosophers[i]->last_meal > data->time_to_die)
			{
				print_status(philosophers[i], "died");
				return ;
			}
			if (philosophers[i]->meals < data->number_of_times_each_philosopher_must_eat)
				full = false;
		}
		if (full)
			return ;
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
	philosophers = init_philosophers(&data);
	start_dinner(philosophers, &data);
	wait_philosophers(philosophers);
	// wait_philosophers(philosophers);
	free_philosophers(philosophers);
}
	
	