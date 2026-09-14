/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 03:50:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/15 03:50:12 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	_think_ms(t_philo *philo);

void	philo_eat(t_philo *philo)
{
	take_both_forks(philo);
	set_last_meal_time(philo, get_time_ms());
	print_status(philo, EATING);
	wait_until(get_time_ms() + philo->ctx->config.time_to_eat);
	add_eat_count(philo);
	put_both_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, SLEEPING);
	wait_until(get_time_ms() + philo->ctx->config.time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, THINKING);
	wait_until(get_time_ms() + _think_ms(philo));
}

static long	_think_ms(t_philo *philo)
{
	long	left_ms;

	left_ms = get_last_meal_time(philo) + philo->ctx->config.time_to_die
		- get_time_ms() - philo->ctx->config.time_to_eat;
	if (left_ms < 0)
		return (0);
	return (left_ms / 2);
}
