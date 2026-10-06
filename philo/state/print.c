/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 06:39:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:35 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Following statements are print pattern.
	- timestamp_in_ms X has taken a fork
	- timestamp_in_ms X is eating
	- timestamp_in_ms X is sleeping
	- timestamp_in_ms X is thinking
	- timestamp_in_ms X died
*/
#include "philo.h"

static const char	*_status_msg(t_status status);

void	print_status(t_philo *philo, t_status status)
{
	long	timestamp_in_ms;

	pthread_mutex_lock(&philo->ctx->dead_mutex);
	if (!philo->ctx->is_dead)
	{
		timestamp_in_ms = get_time_ms() - philo->ctx->start_time;
		printf("%ld %d %s\n", timestamp_in_ms, philo->philo_id,
			_status_msg(status));
	}
	pthread_mutex_unlock(&philo->ctx->dead_mutex);
}

bool	check_death(t_philo *philo)
{
	bool	is_dead;
	long	now;

	pthread_mutex_lock(&philo->ctx->dead_mutex);
	now = get_time_ms();
	is_dead = !is_full(philo) && philo->ctx->config.time_to_die
		<= now - get_last_meal_time(philo);
	if (is_dead)
	{
		philo->ctx->is_dead = true;
		printf("%ld %d %s\n", now - philo->ctx->start_time,
			philo->philo_id, _status_msg(DIED));
	}
	pthread_mutex_unlock(&philo->ctx->dead_mutex);
	return (is_dead);
}

static const char	*_status_msg(t_status status)
{
	if (status == TAKE_FORK)
		return ("has taken a fork");
	else if (status == EATING)
		return ("is eating");
	else if (status == SLEEPING)
		return ("is sleeping");
	else if (status == THINKING)
		return ("is thinking");
	else
		return ("died");
}
