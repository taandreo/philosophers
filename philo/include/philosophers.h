/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:49:13 by tairribe          #+#    #+#             */
/*   Updated: 2024/01/07 15:43:44 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

# define STDIN_FILENO	0	/* standard input file descriptor */
# define STDOUT_FILENO	1	/* standard output file descriptor */
# define STDERR_FILENO	2	/* standard error file descriptor */

typedef enum e_bool
{
	false,
	true
}			t_bool;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				total_meals;
	t_bool			stop;
	long			start_time;
	pthread_mutex_t	print;
	pthread_mutex_t	stop_mutex;
	pthread_t		death_thread;
}	t_data;

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	t_data			*data;
	long			last_meal;
	int				meals;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_lock;
}	t_philosopher;

// utils.c
size_t			ft_strlen(const char *s);
char			ft_is_number(const char *str);
long			ft_strtol(const char *str);
void			*ft_calloc(size_t num, size_t size);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
char			*ft_strdup(const char *s);
void			error(char *msg);
// time.c
long			get_time(void);
long			get_timestamp(long start_time);
// actions.c
void			*routine(void *arg);
// init.c
char			*check_args(int argc, char **argv);
void			read_args(int argc, char **argv, t_data *data);
void			free_philosophers(t_philosopher **philosophers, t_data *data);
t_philosopher	**init_philosophers(t_data *data);
// actions_utils.c
void			print_status(t_philosopher *philosopher, char *status);
void			set_last_meal(t_philosopher *philosopher);
void			set_stop(t_data *data);
t_bool			check_stop(t_philosopher *philosopher);
t_bool			check_death(t_philosopher *philo);

#endif
