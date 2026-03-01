/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:57:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/01 23:51:34 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	free_fork_mutex(int num_of_philo, pthread_mutex_t *fork_mutex);

int	init_ctx(t_ctx *ctx)
{
	int	i;

	ctx->is_dead = false;
	if (pthread_mutex_init(&ctx->dead_mutex, NULL) != 0)
		return (perror("dead_mutex init"), FAILURE);
	if (pthread_mutex_init(&ctx->print_mutex, NULL) != 0)
		return (perror("print_mutex init"),
			pthread_mutex_destroy(&ctx->dead_mutex), FAILURE);
	ctx->fork_mutex = malloc(sizeof(pthread_mutex_t)
			* ctx->config.num_of_philo);
	if (ctx->fork_mutex == NULL)
		return (perror("malloc"), pthread_mutex_destroy(&ctx->dead_mutex),
			pthread_mutex_destroy(&ctx->print_mutex), FAILURE);
	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		if (pthread_mutex_init(&ctx->fork_mutex[i], NULL) != 0)
			return (perror("malloc"), pthread_mutex_destroy(&ctx->dead_mutex),
				pthread_mutex_destroy(&ctx->print_mutex),
				free_fork_mutex(ctx->config.num_of_philo, ctx->fork_mutex),
				FAILURE);
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
