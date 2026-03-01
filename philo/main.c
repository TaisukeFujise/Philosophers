/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:21 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/02 00:38:24 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	int		i;
	int		err;

	if (parse_args(argc, argv, &ctx.config) == FAILURE)
		return (1);
	if (init_ctx(&ctx) == FAILURE)
		return (1);
	i = 0;
	while (i < ctx.config.num_of_philo)
	{
		err = pthread_create(&ctx.philo[i].tid, NULL, philo_action,
				&ctx.philo[i]);
		if (err != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < ctx.config.num_of_philo)
	{
		err = pthread_join(ctx.philo[i].tid, NULL);
		printf("tid = %lu\n", ctx.philo[i].tid);
		if (err != 0)
			return (1);
		i++;
	}
	sleep(1);
	puts("happy");
	return (0);
}
