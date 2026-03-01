/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/01 23:53:47 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	puts("happy");
	return (0);
}
