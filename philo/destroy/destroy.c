/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 04:38:58 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 04:53:50 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutex(t_ctx *ctx)
{
	pthread_mutex_destroy(&ctx->dead_mutex);
	pthread_mutex_destroy(&ctx->print_mutex);
}

void	destroy_fork_mutex(t_ctx *ctx, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&ctx->fork_mutex[i]);
		i++;
	}
	free(ctx->fork_mutex);
}

void	destroy_philo(t_ctx *ctx, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&ctx->philo[i].last_meal_time_mutex);
		i++;
	}
	free(ctx->philo);
}

void	destroy_ctx(t_ctx *ctx)
{
	destroy_mutex(ctx);
	destroy_fork_mutex(ctx, ctx->config.num_of_philo);
	destroy_philo(ctx, ctx->config.num_of_philo);
}
