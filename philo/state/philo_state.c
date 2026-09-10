/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 06:31:20 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 06:48:12 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void set_eat_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->philo_mutex);
}

int get_eat_count(t_philo *philo)
{
	int eat_count;
	pthread_mutex_lock(&philo->philo_mutex);
	eat_count = philo->eat_count;
	pthread_mutex_unlock(&philo->philo_mutex);
	return eat_count;
}

void set_last_meal_time(t_philo *philo, int time_ms)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = time_ms;
	pthread_mutex_unlock(&philo->philo_mutex);
}

int get_last_meal_time(t_philo *philo)
{
	int last_meal_time;

	pthread_mutex_lock(&philo->philo_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (last_meal_time);
}
