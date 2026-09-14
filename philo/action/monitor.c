/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 01:31:49 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 21:50:26 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long		_get_min_meal_time(t_ctx *ctx);
static t_philo	*_find_dead_philo(t_ctx *ctx);

void	monitor_loop(t_ctx *ctx)
{
	long	min_last_meal_time;
	t_philo	*dead_philo;

	while (1)
	{
		min_last_meal_time = _get_min_meal_time(ctx);
		if (min_last_meal_time == -1)
			break ;
		wait_until(min_last_meal_time + ctx->config.time_to_die);
		dead_philo = _find_dead_philo(ctx);
		if (dead_philo != NULL)
		{
			report_death(dead_philo);
			break ;
		}
	}
}

static long	_get_min_meal_time(t_ctx *ctx)
{
	int		i;
	long	min_last_meal_time;
	long	last_meal_time;

	i = 0;
	min_last_meal_time = LONG_MAX;
	while (i < ctx->config.num_of_philo)
	{
		if (!is_full(&ctx->philo[i]))
		{
			last_meal_time = get_last_meal_time(&ctx->philo[i]);
			if (last_meal_time < min_last_meal_time)
				min_last_meal_time = last_meal_time;
		}
		i++;
	}
	if (min_last_meal_time == LONG_MAX)
		return (-1);
	return (min_last_meal_time);
}

static t_philo	*_find_dead_philo(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (!is_full(&ctx->philo[i])
			&& ctx->config.time_to_die <= get_time_ms()
			- get_last_meal_time(&ctx->philo[i]))
			return (&ctx->philo[i]);
		i++;
	}
	return (NULL);
}
