/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwhana21 <iwhana21@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 18:30:58 by iwhana            #+#    #+#             */
/*   Updated: 2022/05/13 14:42:37 by iwhana21         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

struct	s_philo;

typedef struct s_philo_one
{
	int					id;
	int					x_ate;
	int					left_fork_id;
	int					right_fork_id;
	long long			t_last_meal;
	struct s_philo		*philo;
	pthread_t			thread_id;
}	t_philo_one;

typedef struct s_philo
{
	int					nb_philo;
	int					time_death;
	int					time_eat;
	int					time_sleep;
	int					must_eat;
	int					dieded;
	int					all_ate;
	long long			first_timestamp;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		forks[200];
	pthread_mutex_t		writing;
	t_philo_one			philosophers[200];
}	t_philo;

int			ft_atoi(const char *str);
void		action_print(t_philo *philo, int id, char *string);
long long	timestamp(void);
long long	time_diff(long long past, long long pres);
void		smart_sleep(long long time, t_philo *philo);
int			program(t_philo *philo);
void		exit_program(t_philo *philo, t_philo_one *philo_one);
void		death_help(t_philo *philo, t_philo_one *philo_one);

#endif
