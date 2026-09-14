/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:56:38 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 21:52:01 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_args(char **argv);
static bool	_is_valid_number(char *arg);

int	parse_args(int argc, char **argv, t_config *config)
{
	if ((argc != 5) && (argc != 6))
		return (print_error("usage: ./philo n t_die t_eat t_sleep [n_meals]"));
	if (validate_args(argv) == FAILURE)
		return (print_error("arguments must be integers in [1, INT_MAX]"));
	config->num_of_philo = (int)ft_atol(argv[1]);
	config->time_to_die = (int)ft_atol(argv[2]);
	config->time_to_eat = (int)ft_atol(argv[3]);
	config->time_to_sleep = (int)ft_atol(argv[4]);
	if (argc == 6)
		config->must_eat_count = (int)ft_atol(argv[5]);
	else
		config->must_eat_count = -1;
	return (SUCCESS);
}

/*
	validation requirements
	- only number
	- only plus number
	- no 0 for every arg.
	- INT_MAX or less
*/
static int	validate_args(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (_is_valid_number(argv[i]) == false)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static bool	_is_valid_number(char *arg)
{
	long	value;
	int		i;

	if (arg[0] == '\0')
		return (false);
	i = 0;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == false)
			return (false);
		i++;
	}
	errno = 0;
	value = ft_atol(arg);
	if (errno == ERANGE || value < 1 || value > INT_MAX)
		return (false);
	return (true);
}
