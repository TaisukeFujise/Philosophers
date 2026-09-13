/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:39:52 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/13 22:02:11 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	long			time_ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time_ms);
}
