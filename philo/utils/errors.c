/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 03:51:46 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/11 03:51:46 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(const char *msg)
{
	write(2, "philo: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (FAILURE);
}
