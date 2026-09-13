# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/27 04:38:53 by tafujise          #+#    #+#              #
#    Updated: 2026/09/14 01:32:31 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:=philo

SRCS:=main.c\
	utils/errors.c\
	utils/ft_atoi.c\
	utils/ft_atol.c\
	utils/ft_isdigit.c\
	utils/ft_isspace.c\
	utils/ft_strlen.c\
	utils/time.c\
	init/init.c\
	init/parse.c\
	state/dead.c\
	state/philo_state.c\
	state/print.c\
	destroy/destroy.c\
	action/monitor.c\
	action/philo_action.c\

HEADERS:=include/philo.h

OBJDIR:=obj
OBJS:=$(addprefix $(OBJDIR)/,$(SRCS:%.c=%.o))

CC:=cc

# -Iinclude: 各 .c が階層に依らず #include "philo.h" で解決できるようにする。
CFLAGS:=-Wall -Wextra -Werror -pthread -Iinclude

DEBUG_FLAGS:=-g -O0

# ---------------------------------------------------------------------------- #
# デバッグターゲット (debug / tsan / asan) は Linux = 42 の評価環境が前提。
#
# MacBook で使う場合は、リポジトリルートの Dockerfile / compose.yaml で
# Ubuntu コンテナに入ってから make する:
#
#     docker compose build           # 初回のみ
#     docker compose run --rm philo_dev   # /workspace/philo に入る
#
# mac 上で直接叩くと次の制約に当たる:
#   - valgrind / helgrind が存在しない (arm64 mac では実質使えない)
#   - make asan のリーク検出が動かない
#     ("detect_leaks is not supported on this platform")
#
# compose.yaml で seccomp:unconfined を指定しているのは、TSan が起動時に
# personality(ADDR_NO_RANDOMIZE) を呼ぶため。Docker の既定 seccomp が
# これを塞ぐので、外さないと TSan が起動直後に FATAL で落ちる。
# gdb でプロセスにアタッチするために cap_add: SYS_PTRACE も入れている。
# ---------------------------------------------------------------------------- #

# 通常ビルド。タイミングの確認と提出前チェックは必ずこれで行う
# (sanitizer 付きのバイナリで時間の挙動を評価してはいけない)。
all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

# valgrind / gdb 用。-g -O0 を付けるだけで、単体では何も検査しない。
#   valgrind ./philo 5 800 200 200
#     -> 未初期化メモリの読み取り。sanitizer では検出できない唯一の領域 (約20倍遅い)
#   valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200
#     -> リークの詳細な分類 (definitely / indirectly / possibly lost)
#   valgrind --tool=helgrind ./philo 3 800 200 200
#     -> 競合とロック順序違反。make tsan と範囲がほぼ重なるので、
#        TSan が何も出さない時のセカンドオピニオン用 (約100倍遅い)
#   固まった時は別端末から:
#     gdb -p $(pgrep -n philo) -batch -ex "thread apply all bt"
debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

# スレッド関連の検出。make tsan && ./philo 5 800 200 200 で、何も出なければ健全。
# 2種類を同時に見る:
#   1. データ競合 - is_dead, last_meal_time 等をミューテックス無しで触った
#   2. lock-order-inversion - ロック取得順の循環。実際に固まっていなくても
#      「デッドロックしうる」ことを検出できる (philo のフォーク取得順の検証に必須)
# 10倍以上遅いので died が出るのは正常。競合の有無だけを見る。ASan とは併用不可。
tsan: CFLAGS += $(DEBUG_FLAGS) -fsanitize=thread
tsan: re

# メモリエラー (リーク / 領域外 / 解放後使用) と未定義動作の検出。
# make asan && ./philo 5 800 200 200 で、走らせるだけで行番号付きで出る。
# 2〜3倍遅い程度なので普段の開発ビルドにしてよい。TSan とは併用不可。
asan: CFLAGS += $(DEBUG_FLAGS) -fsanitize=address,undefined
asan: re

.PHONY: all clean fclean re debug tsan asan
