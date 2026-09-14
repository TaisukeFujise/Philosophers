/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 23:58:58 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/15 03:12:48 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	_philo_single(t_philo *philo);
static void	_philo_multiple(t_philo *philo);

/*
	philo_action
	- 1人の時
		- `time_to_die`経過後にdie
	- 2人以上の時
		- 左右のindexの大きい方から取得する。
*/
void	*philo_action(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->ctx->config.num_of_philo == 1)
		_philo_single(philo);
	else
		_philo_multiple(philo);
	return (NULL);
}

static void	_philo_single(t_philo *philo)
{
	print_status(philo, THINKING);
	take_fork(philo, 0);
	wait_until(philo->last_meal_time + philo->ctx->config.time_to_die);
	put_fork(philo, 0);
}

static void	_philo_multiple(t_philo *philo)
{
	while (!get_dead(philo->ctx))
	{
		if (is_full(philo))
			break ;
		take_both_forks(philo);
		print_status(philo, EATING);
		wait_until(get_time_ms() + philo->ctx->config.time_to_eat);
		put_both_forks(philo);
		print_status(philo, SLEEPING);
		wait_until(get_time_ms() + philo->ctx->config.time_to_sleep);
		print_status(philo, THINKING);
	}
}
