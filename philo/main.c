/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 05:09:43 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo_threads(t_ctx *ctx);
int	join_philo_threads(t_ctx *ctx);

void	*philo_action(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("id = %d\n", philo->philo_id);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_ctx	ctx;

	if (parse_args(argc, argv, &ctx.config) == FAILURE)
		return (1);
	if (init_ctx(&ctx) == FAILURE)
		return (1);
	if (create_philo_threads(&ctx) == FAILURE)
		return (destroy_ctx(&ctx), 1);
	if (join_philo_threads(&ctx) == FAILURE)
		return (destroy_ctx(&ctx), 1);
	usleep(1000000);
	printf("happy");
	return (destroy_ctx(&ctx), 0);
}

int	create_philo_threads(t_ctx *ctx)
{
	int	i;
	int	err;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		err = pthread_create(&ctx->philo[i].tid, NULL, philo_action,
				&ctx->philo[i]);
		if (err != 0)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	join_philo_threads(t_ctx *ctx)
{
	int	i;
	int	err;

	i = 0;
	while (i < ctx->config.num_of_philo)
	{
		err = pthread_join(ctx->philo[i].tid, NULL);
		printf("tid = %lu\n", ctx->philo[i].tid);
		if (err != 0)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
