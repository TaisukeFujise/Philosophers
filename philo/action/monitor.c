/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 01:31:49 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 02:17:40 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	_get_min_meal_time(t_ctx *ctx);
t_philo	*_find_dead_philo(t_ctx *ctx);

void	monitor_loop(t_ctx *ctx)
{
	long	min_last_meal_time;
	t_philo	*dead_philo;

	// 全部のthreadのmin(last_meal_time) かつ eat_count != must_eat_countを探す
	// last_meal_time + time_to_die = wait_time
	// (i) 全員満腹 = min(last_meal_time)が空
	// -> break (※ philo側で満腹になったらreturnするように設計しているので、set_deadは不要)
	// (ii) min(last_meal_time)がある
	// -> wait_time待つ
	// -> 検査する
	// 		(a) 死亡なし
	// 			continue
	// 		(b) 死亡あり
	// 			report_deatchしてbreak
	while (1)
	{
		min_last_meal_time = _get_min_meal_time(ctx);
		if (min_last_meal_time == -1) // 全員満腹の場合
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

long	_get_min_meal_time(t_ctx *ctx)
{
	int		i;
	long	min_last_meal_time;
	long	last_meal_time;

	i = 0;
	min_last_meal_time = LONG_MAX;
	while (i < ctx->config.num_of_philo)
	{
		last_meal_time = get_last_meal_time(&ctx->philo[i]);
		if (get_eat_count(&ctx->philo[i]) != ctx->config.must_eat_count
			&& last_meal_time < min_last_meal_time)
			min_last_meal_time = last_meal_time;
		i++;
	}
	if (min_last_meal_time == LONG_MAX)
		return (-1);
	return (min_last_meal_time);
}

t_philo	*_find_dead_philo(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (get_eat_count(&ctx->philo[i]) != ctx->config.must_eat_count
			&& ctx->config.time_to_die <= get_time_ms()
			- get_last_meal_time(&ctx->philo[i]))
			return (&ctx->philo[i]);
		i++;
	}
	return (NULL);
}
