/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 06:25:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 06:36:47 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_dead(t_ctx *ctx)
{
	pthread_mutex_lock(&ctx->dead_mutex);
	ctx->is_dead = true;
	pthread_mutex_unlock(&ctx->dead_mutex);
}

bool	get_dead(t_ctx *ctx)
{
	bool	is_dead;

	pthread_mutex_lock(&ctx->dead_mutex);
	is_dead = ctx->is_dead;
	pthread_mutex_unlock(&ctx->dead_mutex);
	return (is_dead);
}
