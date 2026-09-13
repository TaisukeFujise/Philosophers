/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 01:16:39 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		_create_philo_threads(t_ctx *ctx);
int		_join_philo_threads(t_ctx *ctx, int count);

/*
	monitorの実装
*/

int	main(int argc, char **argv)
{
	t_ctx	ctx;

	if (parse_args(argc, argv, &ctx.config) == FAILURE)
		return (1);
	if (init_ctx(&ctx) == FAILURE)
		return (1);
	if (_create_philo_threads(&ctx) == FAILURE)
		return (destroy_ctx(&ctx), 1);
	monitor_loop(&ctx);
	if (_join_philo_threads(&ctx, ctx.config.num_of_philo) == FAILURE)
		return (destroy_ctx(&ctx), 1);
	printf("happy\n");
	return (destroy_ctx(&ctx), 0);
}

void	monitor_loop(t_ctx *ctx)
{
	int		i;
	long	min_last_meal_time;

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
		i = 0;
		while (i < ctx->config.num_of_philo)
		{
			min_last_meal_time = LONG_MAX;
			if (ctx->philo->eat_count != ctx->config.must_eat_count
				&& ctx->philo[i].last_meal_time < min_last_meal_time)
				min_last_meal_time = ctx->philo[i].last_meal_time;
			i++;
		}
		if (min_last_meal_time == LONG_MAX)
			break ;
		usleep((useconds_t)(min_last_meal_time + ctx->config.time_to_die));
		i = 0;
		while (i < ctx->config.num_of_philo)
		{
			if (ctx->config.time_to_die < ctx->philo[i].last_meal_time)
			{
				report_death(&ctx->philo[i]);
				break ;
			}
			i++;
		}
	}
}

int	_create_philo_threads(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (pthread_create(&ctx->philo[i].tid, NULL, philo_action,
				&ctx->philo[i]) != 0)
		{
			set_dead(ctx);
			_join_philo_threads(ctx, i);
			return (print_error("pthread_create failed"));
		}
		i++;
	}
	return (SUCCESS);
}

int	_join_philo_threads(t_ctx *ctx, int count)
{
	int	i;
	int	result;

	i = 0;
	result = SUCCESS;
	while (i < count)
	{
		if (pthread_join(ctx->philo[i].tid, NULL) != 0)
			result = print_error("pthread_join failed");
		i++;
	}
	return (result);
}
