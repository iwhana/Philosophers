/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwhana21 <iwhana21@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:19:34 by iwhana            #+#    #+#             */
/*   Updated: 2022/05/13 14:15:53 by iwhana21         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eats(t_philo_one *philo_one)
{
	t_philo	*philo;

	philo = philo_one->philo;
	pthread_mutex_lock(&(philo->forks[philo_one->left_fork_id]));
	action_print(philo, philo_one->id, "has taken a fork");
	pthread_mutex_lock(&(philo->forks[philo_one->right_fork_id]));
	action_print(philo, philo_one->id, "has taken a fork");
	pthread_mutex_lock(&(philo->meal_check));
	action_print(philo, philo_one->id, "is eating");
	philo_one->t_last_meal = timestamp();
	pthread_mutex_unlock(&(philo->meal_check));
	smart_sleep(philo->time_eat, philo);
	(philo_one->x_ate)++;
	pthread_mutex_unlock(&(philo->forks[philo_one->left_fork_id]));
	pthread_mutex_unlock(&(philo->forks[philo_one->right_fork_id]));
}

void	*p_thread(void *void_philosopher)
{
	int			i;
	t_philo_one	*philo_one;
	t_philo		*philo;

	i = 0;
	philo_one = (t_philo_one *)void_philosopher;
	philo = philo_one->philo;
	if (philo_one->id % 2)
		usleep(15000);
	while (!(philo->dieded))
	{
		philo_eats(philo_one);
		if (philo->all_ate)
			break ;
		action_print(philo, philo_one->id, "is sleeping");
		smart_sleep(philo->time_sleep, philo);
		action_print(philo, philo_one->id, "is thinking");
		i++;
	}
	return (NULL);
}

void	exit_program(t_philo *philo, t_philo_one *philo_one)
{
	int	i;

	i = -1;
	while (++i < philo->nb_philo)
		pthread_join(philo_one[i].thread_id, NULL);
	i = -1;
	while (++i < philo->nb_philo)
		pthread_mutex_destroy(&(philo->forks[i]));
	pthread_mutex_destroy(&(philo->writing));
}

void	death_check(t_philo *philo, t_philo_one *philo_one)
{
	int	i;

	while (!(philo->all_ate))
	{
		i = -1;
		while (++i < philo->nb_philo && !(philo->dieded))
		{
			pthread_mutex_lock(&(philo->meal_check));
			if (time_diff(philo_one[i].t_last_meal, timestamp())
				> philo->time_death)
			{
				action_print(philo, i, "died");
				philo->dieded = 1;
			}
			pthread_mutex_unlock(&(philo->meal_check));
			usleep(100);
		}
		if (philo->dieded)
			break ;
		death_help(philo, philo->philosophers);
	}
}

int	program(t_philo *philo)
{
	int				i;
	t_philo_one		*phi;

	i = 0;
	phi = philo->philosophers;
	philo->first_timestamp = timestamp();
	while (i < philo->nb_philo)
	{
		if (pthread_create(&(phi[i].thread_id), NULL, p_thread, &(phi[i])))
			return (1);
		phi[i].t_last_meal = timestamp();
		i++;
	}
	death_check(philo, philo->philosophers);
	exit_program(philo, phi);
	return (0);
}
