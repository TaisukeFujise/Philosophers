/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 01:57:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	_create_philo_threads(t_ctx *ctx);
int	_join_philo_threads(t_ctx *ctx, int count);

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
