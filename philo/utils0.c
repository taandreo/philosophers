/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 17:27:43 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/04 03:32:20 by tairribe         ###   ########.fr       */
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

void	*ft_calloc(size_t num, size_t size)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	if (__SIZE_MAX__ / size < num)
		return (NULL);
	p = malloc(num * size);
	if (!p)
		return (NULL);
	while (i < num * size)
	{
		p[i] = 0;
		i++;
	}
	return ((void *) p);
}

void	error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}