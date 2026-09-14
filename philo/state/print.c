/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 06:39:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 21:09:40 by tafujise         ###   ########.fr       */
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

void	print_status(t_philo *philo, const char *status)
{
	long	timestamp_in_ms;

	pthread_mutex_lock(&philo->ctx->print_mutex);
	pthread_mutex_lock(&philo->ctx->dead_mutex);
	if (!philo->ctx->is_dead)
	{
		timestamp_in_ms = get_time_ms() - philo->ctx->start_time;
		printf("%ld %d %s\n", timestamp_in_ms, philo->philo_id,
			_status_msg(status));
	}
	pthread_mutex_unlock(&philo->ctx->dead_mutex);
	pthread_mutex_unlock(&philo->ctx->print_mutex);
}

void	report_death(t_philo *philo)
{
	long	timestamp_in_ms;

	pthread_mutex_lock(&philo->ctx->print_mutex);
	set_dead(philo->ctx);
	timestamp_in_ms = get_time_ms() - philo->ctx->start_time;
	printf("%ld %d %s\n", timestamp_in_ms, philo->philo_id, "died");
	pthread_mutex_unlock(&philo->ctx->print_mutex);
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
