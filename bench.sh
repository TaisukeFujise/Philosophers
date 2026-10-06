#!/usr/bin/env bash
# philo の計測ハーネス。
#
#   ./bench.sh                      既定ケースを既定回数
#   ./bench.sh 50                   既定ケースを50回ずつ
#   ./bench.sh 50 "4 410 200 200 10"    指定ケースだけ
#
# 合否の定義:
#   die_at -> died が出る。かつ時刻が [time_to_die, time_to_die + 10] に入る
#             (最初の締切で死ぬケース専用)
#   die    -> died が出る。時刻は問わない (数周してから落ちるケース)
#   nodie  -> died が出ない。exit 0。全員が must_eat_count に到達する
#
# 失敗した回は out/ に生ログを残し、各哲学者の食事間隔表を添える。

set -u

cd "$(dirname "$0")/philo" || exit 1

RUNS=${1:-30}
OUT=../out
TIMEOUT=60

# expectation|args
DEFAULT_CASES=(
	"die_at|1 800 200 200 10"
	"die_at|4 310 200 100 10"
	"die|5 600 200 200"
	"nodie|2 800 200 200 5"
	"nodie|3 800 200 200 7"
	"nodie|4 410 200 200 10"
	"nodie|5 800 200 200 7"
	"nodie|7 800 200 200 7"
	"nodie|100 800 200 200 3"
	"nodie|200 800 200 200 2"
)

if [ $# -ge 2 ]; then
	CASES=()
	shift
	for a in "$@"; do CASES+=("auto|$a"); done
else
	CASES=("${DEFAULT_CASES[@]}")
fi

if [ -t 1 ]; then R='\033[31m'; G='\033[32m'; Z='\033[0m'; else R=; G=; Z=; fi

[ -x ./philo ] || { echo "philo が無い。make してから実行する"; exit 1; }
mkdir -p "$OUT"; rm -f "$OUT"/fail-*.log

# 食事開始の間隔表。締切を超えた間隔に << をつける
intervals() {
	awk -v die="$1" '
	/ is eating$/ { id=$2; n[id]++; if (n[id] > 1) { d = $1 - prev[id];
			s[id] = s[id] sprintf("%d%s ", d, (d > die ? "<<" : "")) } prev[id]=$1 }
	END { for (i=1; i<=length(n); i++)
			printf "    philo %-3s 食事%2d回  間隔: %s\n", i, n[i], s[i] }' "$2"
}

printf '%-24s %-6s %s\n' "CASE" "RESULT" "DETAIL"
printf '%.0s-' {1..72}; echo

total_fail=0
for entry in "${CASES[@]}"; do
	expect=${entry%%|*}
	args=${entry#*|}
	set -- $args
	n_philo=$1; t_die=$2; meals=${5:-}
	[ "$expect" = auto ] && { expect=nodie; [ -z "$meals" ] && expect=die; }

	fail=0; first_detail=""
	for i in $(seq "$RUNS"); do
		log=$(mktemp)
		timeout "$TIMEOUT" ./philo $args > "$log" 2>&1
		code=$?
		died=$(grep -m1 ' died$' "$log")
		bad=""
		if [ "$expect" = die ] || [ "$expect" = die_at ]; then
			if [ -z "$died" ]; then bad="死者が出ない"
			else
				t=${died%% *}
				[ "$t" -lt "$t_die" ] && bad="死亡が早すぎる (${t}ms < ${t_die})"
				[ "$expect" = die_at ] && [ "$t" -gt $((t_die + 10)) ] \
					&& bad="死亡が遅すぎる (${t}ms > $((t_die + 10)))"
			fi
		else
			if [ -n "$died" ]; then bad="死者が出た: $died"
			elif [ "$code" -ne 0 ]; then bad="exit=$code (タイムアウト/異常終了)"
			elif [ -n "$meals" ]; then
				short=$(grep -c ' is eating$' "$log")
				[ "$short" -lt $((n_philo * meals)) ] \
					&& bad="食事回数が足りない ($short < $((n_philo * meals)))"
			fi
		fi
		if [ -n "$bad" ]; then
			fail=$((fail + 1))
			if [ "$fail" -le 3 ]; then
				f="$OUT/fail-$(echo "$args" | tr ' ' '_')-$i.log"
				{ echo "# ./philo $args   (run $i): $bad"; echo "# --- 各哲学者の食事間隔 (<< は time_to_die 超過) ---"
				  intervals "$t_die" "$log"; echo "# --- 死亡直前30行 ---"; tail -30 "$log"; } > "$f"
			fi
			[ -z "$first_detail" ] && first_detail="$bad"
		fi
		rm -f "$log"
	done

	total_fail=$((total_fail + fail))
	if [ "$fail" -eq 0 ]; then
		printf "%-24s ${G}%-6s${Z} %d/%d\n" "$args" "PASS" "$RUNS" "$RUNS"
	else
		printf "%-24s ${R}%-6s${Z} %d/%d 失敗  %s\n" \
			"$args" "FAIL" "$fail" "$RUNS" "$first_detail"
	fi
done

echo
if [ "$total_fail" -eq 0 ]; then
	echo "全ケース PASS ($RUNS 回ずつ)"
else
	echo "失敗 $total_fail 件。詳細: $OUT/fail-*.log"
	echo "注: 1割で落ちる事象を「消えた」と言うには 30回以上 (0.9^30 = 0.04) 必要。"
fi
