/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/28 16:33:06 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(int argc, char **argv, t_config *config)
{
	if ((argc != 5) && (argc != 6))
		return (write(2, "Error1\n", 6), FAILURE);
	// if (validate_args(argv) == FAILURE)
	// 	return (write(2, "Error2\n", 6), FAILURE);
	config->num_of_philo = ft_atoi(argv[1]);
	config->time_to_die = ft_atol(argv[2]);
	config->time_to_eat = ft_atol(argv[3]);
	config->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		config->must_eat_count = ft_atoi(argv[5]);
	else
		config->must_eat_count = -1;
	return (SUCCESS);
}

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
		// if (pthread_mutex_init(&ctx->fork_mutex[i], NULL) != 0)
		// 	return ()
	}
	return (SUCCESS);
}

// void	*philo_action(void *arg)
// {
// }

int	main(int argc, char **argv)
{
	t_ctx	ctx;

	// int		err;
	if (parse_args(argc, argv, &ctx.config) == FAILURE)
		return (1);
	if (init_ctx(&ctx) == FAILURE)
		return (1);
	// err = pthread_create(&tid, NULL, philo_action, NULL);
	return (0);
}
