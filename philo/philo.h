/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:25 by tafujise          #+#    #+#             */
/*   Updated: 2026/03/02 00:00:16 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define FAILURE -1

typedef struct s_config
{
	int					num_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
}						t_config;

typedef struct s_ctx	t_ctx;

typedef struct s_philo
{
	pthread_t			tid;
	int					philo_id;
	int					eat_count;
	int					last_meal_time;
	pthread_mutex_t		last_meal_time_mutex;
	t_ctx				*ctx;
}						t_philo;

struct					s_ctx
{
	t_config			config;
	int					start_time;
	bool				is_dead;
	pthread_mutex_t		dead_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*fork_mutex;
	t_philo				*philo;
};

// <utils>
/* ft_atoi.c */
int						ft_atoi(const char *nptr);
/* ft_is_digit.c */
int						ft_isdigit(int c);
/* ft_isspace.c */
int						ft_isspace(int c);

// <init>
/* init.c */
int						init_ctx(t_ctx *ctx);
/* parse.c */
int						parse_args(int argc, char **argv, t_config *config);

#endif
