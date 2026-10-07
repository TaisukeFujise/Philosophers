/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/10/07 23:58:49 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	_create_philo_threads(t_ctx *ctx);
static int	_join_philo_threads(t_ctx *ctx, int count);

/*
	./philo
	- av[1]: num_of_philo
	- av[2]: time_to_die
	- av[3]: time_to_eat
	- av[4]: time_to_sleep
	- av[5]: must_eat_count(optional)
	ex) ./philo 4 200 100 100
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
	return (destroy_ctx(&ctx), 0);
}

static int	_create_philo_threads(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (pthread_create(&ctx->philo[i].tid, NULL, philo_action,
				&ctx->philo[i]))
		{
			set_dead(ctx);
			_join_philo_threads(ctx, i);
			return (print_error("pthread_create failed"));
		}
		i++;
	}
	return (SUCCESS);
}

static int	_join_philo_threads(t_ctx *ctx, int count)
{
	int	i;
	int	result;

	i = 0;
	result = SUCCESS;
	while (i < count)
	{
		if (pthread_join(ctx->philo[i].tid, NULL))
			result = print_error("pthread_join failed");
		i++;
	}
	return (result);
}
