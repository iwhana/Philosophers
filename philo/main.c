/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwhana21 <iwhana21@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:20:40 by iwhana            #+#    #+#             */
/*   Updated: 2022/05/13 14:18:05 by iwhana21         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(t_philo *philo)
{
	int	i;

	i = philo->nb_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(philo->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(philo->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(philo->meal_check), NULL))
		return (1);
	return (0);
}

int	init_philosophers(t_philo *philo)
{
	int	i;

	i = philo->nb_philo;
	while (--i >= 0)
	{
		philo->philosophers[i].id = i;
		philo->philosophers[i].x_ate = 0;
		philo->philosophers[i].left_fork_id = i;
		philo->philosophers[i].right_fork_id = (i + 1) % philo->nb_philo;
		philo->philosophers[i].t_last_meal = 0;
		philo->philosophers[i].philo = philo;
	}
	return (0);
}

void	death_help(t_philo *philo, t_philo_one *philo_one)
{
	int	i;

	i = 0;
	while (philo->must_eat != -1 && i < philo->nb_philo
		&& philo_one[i].x_ate >= philo->must_eat)
		i++;
	if (i == philo->nb_philo)
		philo->all_ate = 1;
}

static void	main_help(t_philo *philo, char **argv)
{
	if (argv[5])
	{
		philo->must_eat = ft_atoi(argv[5]);
		if (philo->must_eat <= 0)
			return ;
	}
	else
		philo->must_eat = -1;
}

int	main(int argc, char **argv)
{
	t_philo	philo;

	if (argc != 5 && argc != 6)
		return (1);
	philo.nb_philo = ft_atoi(argv[1]);
	philo.time_death = ft_atoi(argv[2]);
	philo.time_eat = ft_atoi(argv[3]);
	philo.time_sleep = ft_atoi(argv[4]);
	philo.all_ate = 0;
	philo.dieded = 0;
	if (philo.nb_philo < 2 || philo.time_death < 0 || philo.time_eat < 0
		|| philo.time_sleep < 0)
		return (1);
	main_help(&philo, argv);
	if (init_mutex(&philo))
		return (2);
	init_philosophers(&philo);
	if (program(&philo))
		return (1);
	return (0);
}
