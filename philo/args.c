/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 20:50:03 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/03 00:10:38 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	read_args(int argc, char **argv, t_data *data)
{
	data->number_of_philosophers = ft_strtol(argv[1]);
	data->time_to_die = ft_strtol(argv[2]);
	data->time_to_eat = ft_strtol(argv[3]);
	data->time_to_sleep = ft_strtol(argv[4]);
	if (argc == 6)
		data->total_meals = ft_strtol(argv[5]);
	else
		data->total_meals = -1;
}
