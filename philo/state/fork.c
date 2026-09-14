/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 22:41:16 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/15 02:46:07 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_fork(t_philo *philo, int fork_id)
{
	pthread_mutex_lock(&philo->ctx->fork_mutex[fork_id]);
	print_status(philo, TAKE_FORK);
}

void	put_fork(t_philo *philo, int fork_id)
{
	pthread_mutex_unlock(&philo->ctx->fork_mutex[fork_id]);
}

void	take_both_forks(t_philo *philo)
{
	if (philo->left < philo->right)
	{
		take_fork(philo, philo->right);
		take_fork(philo, philo->left);
	}
	else
	{
		take_fork(philo, philo->left);
		take_fork(philo, philo->right);
	}
}

void	put_both_forks(t_philo *philo)
{
	put_fork(philo, philo->right);
	put_fork(philo, philo->left);
}
