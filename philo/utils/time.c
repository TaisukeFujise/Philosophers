/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:39:52 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 02:06:28 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	long			time_ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time_ms);
}

void	wait_until(long deadline_ms)
{
	long	cur_time_ms;
	long	duration_us;

	cur_time_ms = get_time_ms();
	while (cur_time_ms < deadline_ms)
	{
		duration_us = (deadline_ms - cur_time_ms) * 1000;
		if (duration_us > WAIT_STEP_US)
			duration_us = WAIT_STEP_US;
		usleep((useconds_t)duration_us);
		cur_time_ms = get_time_ms();
	}
}

long	get_think_ms(t_config *config)
{
	long	think_ms;

	think_ms = (config->time_to_die - config->time_to_eat
			- config->time_to_sleep) / 2;
	if (think_ms < 0)
		return (0);
	return (think_ms);
}
