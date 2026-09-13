/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:46:25 by tafujise          #+#    #+#             */
/*   Updated: 2026/09/14 01:48:03 by tafujise         ###   ########.fr       */
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

/*
	wait_until() が 1 回の usleep に渡す上限 (マイクロ秒)。
	get_time_ms() の分解能が 1ms なので、それより長く寝ても
	締切の判定精度は上がらない。調整用の値ではなく時計の仕様から決まる。
	usleep に 1,000,000 以上を渡したときの挙動は POSIX 上未定義だが、
	この上限により構造的に到達しない。
*/
# define WAIT_STEP_US 1000

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
	long				last_meal_time;
	pthread_mutex_t		philo_mutex;
	t_ctx				*ctx;
}						t_philo;

struct					s_ctx
{
	t_config			config;
	long				start_time;
	bool				is_dead;
	pthread_mutex_t		dead_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*fork_mutex;
	t_philo				*philo;
};

// <utils>
/* time.c */
long					get_time_ms(void);
void					wait_until(long deadline_ms);
/* errors.c */
int						print_error(const char *msg);
/* ft_atoi.c */
int						ft_atoi(const char *nptr);
/* ft_atol.c */
long					ft_atol(char *nptr);
/* ft_isdigit.c */
int						ft_isdigit(int c);
/* ft_isspace.c */
int						ft_isspace(int c);
/* ft_strlen.c */
size_t					ft_strlen(const char *str);

// <init>
/* init.c */
int						init_ctx(t_ctx *ctx);
/* parse.c */
int						parse_args(int argc, char **argv, t_config *config);

// <state>
/* dead.c */
void					set_dead(t_ctx *ctx);
bool					get_dead(t_ctx *ctx);
/* philo_state.c */
void					add_eat_count(t_philo *philo);
int						get_eat_count(t_philo *philo);
void					set_last_meal_time(t_philo *philo, long time_ms);
long					get_last_meal_time(t_philo *philo);
/* print.c */
void					print_status(t_philo *philo, const char *status);
void					report_death(t_philo *philo);

// <destroy>
/* destroy.c */
void					destroy_mutex(t_ctx *ctx);
void					destroy_fork_mutex(t_ctx *ctx, int count);
void					destroy_philo(t_ctx *ctx, int count);
void					destroy_ctx(t_ctx *ctx);

// <action>
/* monitor.c */
void					monitor_loop(t_ctx *ctx);
/* philo_action.c */
void					*philo_action(void *arg);

#endif
