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

void	destroy_fork_mutex(int num_of_philo, pthread_mutex_t *fork_mutex)
{
	int	i;

	i = 0;
	while (i < num_of_philo)
	{
		pthread_mutex_destroy(&fork_mutex[i]);
		i++;
	}
	free(fork_mutex);
}

void	destroy_philo(int num_of_philo, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < num_of_philo)
	{
		pthread_mutex_destroy(&philo[i].last_meal_time_mutex);
		i++;
	}
	free(philo);
}

void	destroy_ctx(t_ctx *ctx)
{
	destroy_mutex(ctx);
	destroy_fork_mutex(ctx->config.num_of_philo, ctx->fork_mutex);
	destroy_philo(ctx->config.num_of_philo, ctx->philo);
}
