/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 03:50:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/10/06 23:30:15 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	take_both_forks(philo);
	set_last_meal_time(philo, get_time_us());
	print_status(philo, EATING);
	wait_until(get_time_us() + philo->ctx->config.time_to_eat);
	add_eat_count(philo);
	put_both_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, SLEEPING);
	wait_until(get_time_us() + philo->ctx->config.time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, THINKING);
	wait_until(get_time_us() + get_think_us(&philo->ctx->config));
}
