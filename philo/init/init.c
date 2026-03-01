/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:57:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/02 00:35:06 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int		init_fork_mutex(t_ctx *ctx);
int		init_philo(t_ctx *ctx);
void	free_fork_mutex(int num_of_philo, pthread_mutex_t *fork_mutex);

int	init_ctx(t_ctx *ctx)
{
	ctx->is_dead = false;
	if (pthread_mutex_init(&ctx->dead_mutex, NULL) != 0)
		return (perror("dead_mutex init"), FAILURE);
	if (pthread_mutex_init(&ctx->print_mutex, NULL) != 0)
		return (perror("print_mutex init"),
			pthread_mutex_destroy(&ctx->dead_mutex), FAILURE);
	if (init_fork_mutex(ctx) == FAILURE)
		return (pthread_mutex_destroy(&ctx->dead_mutex),
			pthread_mutex_destroy(&ctx->print_mutex), FAILURE);
	if (init_philo(ctx) == FAILURE)
		return (pthread_mutex_destroy(&ctx->dead_mutex),
			pthread_mutex_destroy(&ctx->print_mutex),
			free_fork_mutex(ctx->config.num_of_philo, ctx->fork_mutex),
			FAILURE);
	return (SUCCESS);
}

int	init_fork_mutex(t_ctx *ctx)
{
	int	i;

	ctx->fork_mutex = malloc(sizeof(pthread_mutex_t)
			* ctx->config.num_of_philo);
	if (ctx->fork_mutex == NULL)
		return (FAILURE);
	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (pthread_mutex_init(&ctx->fork_mutex[i], NULL) != 0)
			return (free_fork_mutex(i, ctx->fork_mutex), FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	init_philo(t_ctx *ctx)
{
	int	i;
	int	j;

	ctx->philo = malloc(sizeof(t_philo) * ctx->config.num_of_philo);
	if (ctx->philo == NULL)
		return (FAILURE);
	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		ctx->philo[i].philo_id = i;
		ctx->philo[i].eat_count = 0;
		ctx->philo[i].last_meal_time = 0;
		ctx->philo[i].ctx = ctx;
		if (pthread_mutex_init(&ctx->philo[i].last_meal_time_mutex, NULL) != 0)
		{
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(&ctx->philo[j].last_meal_time_mutex);
				j++;
			}
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

void	free_fork_mutex(int num_of_philo, pthread_mutex_t *fork_mutex)
{
	int	i;

	i = 0;
	while (i < num_of_philo)
	{
		pthread_mutex_destroy(fork_mutex);
		i++;
	}
	free(fork_mutex);
}
