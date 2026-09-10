/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 06:39:42 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 07:18:37 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

#include "philo.h"

void print_status(t_philo *philo, const char *status)
{
	/* TODO */
	// int timestamp_in_ms;

	// pthread_mutex_lock(&philo->ctx->print_mutex);
	// pthread_mutex_lock(&philo->ctx->dead_mutex);
	// if (philo->ctx->is_dead) {
	// 	print_death(philo);
	// } else {
	// 	timestamp_in_ms = gettimeofday(NULL, NULL);
	// 	printf("%d %d %s", timestamp_in_ms, philo->philo_id, status);
	// }
	// pthread_mutex_unlock(&philo->ctx->dead_mutex);
	// pthread_mutex_unlock(&philo->ctx->print_mutex);
}

void print_death(t_philo *philo)
{
	/* TODO */
}
