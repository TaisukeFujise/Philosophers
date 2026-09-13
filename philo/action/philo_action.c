/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 23:58:58 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/13 23:59:09 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_action(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	printf("id = %d\n", philo->philo_id);
	return (NULL);
}
