/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 20:56:38 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/01 23:59:54 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	validate_args(char **argv);

int	parse_args(int argc, char **argv, t_config *config)
{
	if ((argc != 5) && (argc != 6))
		return (write(2, "Error1\n", 6), FAILURE);
	if (validate_args(argv) == FAILURE)
		return (write(2, "Error2\n", 6), FAILURE);
	config->num_of_philo = ft_atoi(argv[1]);
	if (config->num_of_philo == 0)
		return (FAILURE);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		config->must_eat_count = ft_atoi(argv[5]);
	else
		config->must_eat_count = -1;
	return (SUCCESS);
}

/*
	validation requirements
	- only number
	- only plus number
	- no 0 for a number_of_philosopher arg.
*/
int	validate_args(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isdigit(argv[i][j]) == false)
				return (FAILURE);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
