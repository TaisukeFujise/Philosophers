/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:57:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 04:54:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		_init_mutex(t_ctx *ctx);
int		_init_fork_mutex(t_ctx *ctx);
int		_init_philo(t_ctx *ctx);

int	init_ctx(t_ctx *ctx)
{
	ctx->is_dead = false;
	if (_init_mutex(ctx) == FAILURE)
		return (FAILURE);
	if (_init_fork_mutex(ctx) == FAILURE)
		return (destroy_mutex(ctx),
			FAILURE);
	if (_init_philo(ctx) == FAILURE)
		return (destroy_mutex(ctx),
			destroy_fork_mutex(ctx->config.num_of_philo, ctx->fork_mutex),
			FAILURE);
	return (SUCCESS);
}

int _init_mutex(t_ctx *ctx)
{
	if (pthread_mutex_init(&ctx->dead_mutex, NULL) != 0)
		return (put_error("dead_mutext init failed"));
	if (pthread_mutex_init(&ctx->print_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&ctx->dead_mutex),
			put_error("print_mutex init failed"));
	return (SUCCESS);
}

int _init_fork_mutex(t_ctx *ctx) {
	int	i;

	ctx->fork_mutex = malloc(sizeof(pthread_mutex_t)
			*ctx->config.num_of_philo);
	if (ctx->fork_mutex == NULL)
		return (put_error("malloc failed: fork mutexes"));
	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (pthread_mutex_init(&ctx->fork_mutex[i], NULL) != 0)
			return (destroy_fork_mutex(i, ctx->fork_mutex),
				put_error("fork mutex init failed"));
		i++;
	}
	return (SUCCESS);
}

int	_init_philo(t_ctx *ctx)
{
	int	i;

	ctx->philo = malloc(sizeof(t_philo) * ctx->config.num_of_philo);
	if (ctx->philo == NULL)
		return (put_error("malloc failed: philosophers"));
	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		ctx->philo[i].philo_id = i;
		ctx->philo[i].eat_count = 0;
		ctx->philo[i].last_meal_time = 0;
		ctx->philo[i].ctx = ctx;
		if (pthread_mutex_init(&ctx->philo[i].last_meal_time_mutex, NULL) != 0)
			return (destroy_philo(i, ctx->philo),
				put_error("last_meal_time mutex init failed"));
		i++;
	}
	return (SUCCESS);
}
