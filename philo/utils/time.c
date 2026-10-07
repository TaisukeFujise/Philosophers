/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:39:52 by tafujise          #+#    #+#             */
/*   Updated: 2026/10/07 23:43:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_us(void)
{
	long			time_us;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time_us = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	return (time_us);
}

void	wait_until(long deadline_us)
{
	long	cur_time_us;
	long	duration_us;

	cur_time_us = get_time_us();
	while (cur_time_us < deadline_us)
	{
		duration_us = deadline_us - cur_time_us;
		if (duration_us > WAIT_STEP_US)
			duration_us = WAIT_STEP_US;
		usleep((useconds_t)duration_us);
		cur_time_us = get_time_us();
	}
}

long	get_think_us(t_config *config)
{
	long	think_us;
	long	eat_time;
	long	sleep_time;

	eat_time = config->time_to_eat;
	sleep_time = config->time_to_sleep;
	think_us = MAX(config->cycle_us, eat_time + sleep_time) - eat_time
		- sleep_time;
	return (think_us);
}
