/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:13:37 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/28 16:33:29 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	round_int_limits(long int num)
{
	if (num < INT_MIN)
		return (INT_MIN);
	if (INT_MAX < num)
		return (INT_MAX);
	return ((int)num);
}

int	ft_atoi(const char *nptr)
{
	int			symbol;
	long int	num;

	symbol = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			symbol *= -1;
		nptr++;
	}
	if (ft_isdigit(*nptr) == 0)
		return (0);
	num = *nptr - '0';
	nptr++;
	while (ft_isdigit(*nptr))
	{
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (round_int_limits(num * symbol));
}
