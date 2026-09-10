# デバッグツールの使い分け

Philosophers のデバッグ環境と、各ツールの使い分けのメモ。
42 の評価環境は Ubuntu なので、mac で開発する場合は Docker で Ubuntu を再現して使う。

---

## 1. 起動

```bash
docker compose build                  # 初回のみ（約800MB のイメージができる）
docker compose run --rm philo_dev     # /workspace/philo に入る
```

以降のコマンドは全てこのコンテナ内で実行する。

- ソースは `.:/workspace` でバインドマウントされているので、**ホストの VSCode で編集 → コンテナで `make`** がそのまま動く。`obj/` や `philo` バイナリもホスト側の実体。
- コンテナ内で `apt-get install` したものは抜けると消える。常用するツールは `Dockerfile` に追記する。
- 抜けるときは **`exit` か Ctrl-D**。コンテナは自動で削除される（`--rm`）。
  - `--rm` は「PID 1 の `bash` が終了した瞬間」に発動する。`./philo` は bash の子プロセスなので、実行中にコンテナが消えることはない。
  - **端末のウィンドウを閉じて終わらせるとコンテナが残り続ける**（bash が終了しないため）。残骸の確認と掃除は下記。

```bash
docker ps -a -f name=philosophers     # 何か出たら孤児コンテナ
docker compose down                    # 片付け
```

### compose.yaml で指定している実行時オプション

| 設定 | 理由 |
|---|---|
| `security_opt: seccomp:unconfined` | TSan が起動時に `personality(ADDR_NO_RANDOMIZE)` を呼ぶ。Docker の既定 seccomp がこれを塞ぐので、外さないと TSan が `FATAL: ThreadSanitizer CHECK failed` で即死する |
| `cap_add: SYS_PTRACE` | gdb で動作中プロセスにアタッチするために必要 |
| `platform: linux/amd64`（コメントアウト） | 42 は x86_64。厳密に合わせたい時だけ有効化する（エミュレーションで遅くなる） |

---

## 2. 使い分けの早見表

```bash
# リーク・メモリ確認（主軸）
make debug && valgrind --leak-check=full ./philo 5 800 200 200

# 競合・デッドロック確認
make tsan && ./philo 5 800 200 200

# 未初期化メモリの確認
make debug && valgrind ./philo 5 800 200 200

# 固まった時（別端末 or バックグラウンド実行して）
gdb -p $(pgrep -n philo) -batch -ex "thread apply all bt"

# 提出前・タイミング確認
make re && ./philo 200 800 200 200
```

**重要: sanitizer 付きのバイナリでタイミングを評価しないこと。** TSan は10倍以上遅いので `died` が出るのは正常。時間の挙動を見るときは必ず `make re` で素のビルドに戻す。

---

## 3. 各ツール

### make debug + valgrind memcheck ← リーク確認の主軸

```bash
make debug
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200
```

```
    in use at exit: 640 bytes in 2 blocks
240 bytes in 1 blocks are definitely lost in loss record 1 of 2
   by init_fork_mutex (init.c:42)
   by init_ctx (init.c:27)
   by main (main.c:32)
LEAK SUMMARY:
   definitely lost: 640 bytes in 2 blocks
   indirectly lost: 0 bytes in 0 blocks
```

- `definitely lost` / `indirectly lost` / `possibly lost` に分類してくれる
- スタックトレースにサニタイザ自身のフレームが混ざらないので読みやすい
- 修正できたら `All heap blocks were freed -- no leaks are possible` が出る
- **未初期化メモリの読み取りを検出できる唯一のツール**（後述）。`--leak-check` なしの素の `valgrind ./philo ...` でも出る
- 約20倍遅い

哲学者の数を変えるとリークのサイズが変わるので、何の確保か特定できる。

```
philo 3  -> definitely lost: 384 bytes    (3*48 + 3*80)
philo 5  -> definitely lost: 640 bytes    (5*48 + 5*80)
philo 10 -> definitely lost: 1,280 bytes  (10*48 + 10*80)
```

（コンテナ内で `sizeof(pthread_mutex_t)` = 48、`sizeof(t_philo)` = 80）

### make tsan ← スレッド関連の主軸

```bash
make tsan
./philo 5 800 200 200
```

**何も出力されなければ健全。** 2種類を同時に見る。

**① データ競合** — `is_dead` や `last_meal_time` をミューテックス無しで触ると出る。

```
WARNING: ThreadSanitizer: data race
  Read of size 4 at 0x... by thread T2:
    #0 monitor philo.c:42
  Previous write of size 4 at 0x... by thread T1:
    #0 eat philo.c:88
  Location is heap block of size 240
```

**② lock-order-inversion（デッドロックの予測）** — ロック取得順に循環があると、**実際に固まっていなくても**検出できる。フォーク取得順（偶数は左→右、奇数は右→左 等）の検証に必須。

```
WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock)
  Cycle in lock order graph: M0 => M1 => M0

  Mutex M1 acquired here while holding mutex M0 in thread T1:
    #1 t1 lockorder.c:5

  Mutex M0 acquired here while holding mutex M1 in thread T2:
    #1 t2 lockorder.c:8
```

- 10倍以上遅い。`died` が出るのは正常なので、競合の有無だけを見る
- ASan とは併用不可（両方ともメモリレイアウトを掌握するため）

### make asan ← セグフォの原因を速く知りたい時

```bash
make asan
./philo 5 800 200 200
```

走らせるだけで、正常終了後にリークが出る。領域外アクセスや解放後使用も検出する。

```
==44==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 400 byte(s) in 1 object(s) allocated from:
    #1 in init_philo init/init.c:61
    #2 in init_ctx init/init.c:30
    #3 in main main.c:32
SUMMARY: AddressSanitizer: 640 byte(s) leaked in 2 allocation(s).
```

- **2〜3倍遅い程度**なので、普段の開発ビルドにしてもよい
- valgrind と違い、**スタック・グローバル変数の領域外**も検出できる
- `-fsanitize=address,undefined` なので UBSan（整数オーバーフロー、ゼロ除算等）も同時に有効
- **mac 上では動かない**（`detect_leaks is not supported on this platform`）。必ずコンテナ内で

### gdb ← 実際に固まった時

**helgrind / TSan が「デッドロックしうる」の予測、gdb が「今どこで止まっているか」の現行犯確認。**

同じシェル内でバックグラウンド実行するのが手軽。

```bash
./philo 200 800 200 200 &
gdb -p $(pgrep -n philo) -batch -ex "thread apply all bt"
```

```
Thread 3: ___pthread_mutex_lock (mutex=0x...018 <a>) at t2 (dl.c:5)
Thread 2: ___pthread_mutex_lock (mutex=0x...048 <b>) at t1 (dl.c:4)
```

どのスレッドが**どのミューテックス**を待っているか、変数名付きで出る。

別端末を使う場合は、**ハングしているプロセスと同じコンテナ**に入る必要がある。

```bash
docker exec -it $(docker ps -q -f name=philosophers) bash
```

`docker compose exec philo_dev bash` は、`docker compose up` のコンテナが同時に存在するとそちら側に入ってしまい、`pgrep philo` が何も返さなくなる。`run --rm` だけで運用していれば問題ない。

### helgrind ← セカンドオピニオン

```bash
make debug
valgrind --tool=helgrind ./philo 3 800 200 200
```

TSan とカバー範囲がほぼ重なる（競合 + ロック順序違反）。**約100倍遅い**ので、TSan が何も出さないのに挙動が怪しい時だけ使う。3〜5人の小さい引数で。

valgrind は libc の中まで見えるので、TSan が見ない領域も報告する。`Locks held: none` は「ミューテックスを取らずに共有データを触った」の意味。

```
Possible data race during write of size 1 at 0x4A3F397 by thread #3
Locks held: none
   at __GI_memcpy
   by _IO_new_file_xsputn (fileops.c:1235)
   by printf (printf.c:33)
   by philo_action (main.c:20)
```

↑ `print_mutex` を取らずに `printf` した場合の典型的な出力。

### norminette

```bash
norminette *.c *.h init utils
```

ビルドと同じコンテナで回せるので、提出前チェックが1コマンドで済む。

---

## 4. カバー範囲の対応表

太字が「片方にしかできないこと」。

| 検出対象 | ASan+UBSan | TSan | memcheck | helgrind |
|---|---|---|---|---|
| メモリリーク | ○ | × | ○ | × |
| ヒープ領域外アクセス | ○ | × | ○ | × |
| **スタック/グローバル領域外** | **○** | × | **×** | × |
| 解放後使用・二重解放 | ○ | × | ○ | × |
| **未初期化メモリの読み取り** | **×** | × | **○** | × |
| 整数オーバーフロー等の未定義動作 | ○ | × | × | × |
| データ競合 | × | ○ | × | ○ |
| ロック順序違反 | × | ○ | × | ○ |
| **速度** | **2〜3倍** | 10倍 | 20倍 | 100倍 |

### 未初期化メモリは memcheck だけ

```c
int *p = malloc(sizeof(int) * 4);
if (p[2] == 42)        /* 未初期化 */
```

```
ASan+UBSan の報告件数: 0
memcheck: Conditional jump or move depends on uninitialised value(s)
          at main (uninit.c:5)
```

ASan は完全に見逃す。philo だと「`t_philo` のメンバを初期化し忘れた」「`pthread_mutex_init` を忘れたミューテックスを使った」がここに刺さる。**valgrind を使う一番の理由。**

---

## 5. 仕組みの違い（なぜ性質が違うのか）

### Sanitizer = コンパイル時計装

コンパイラがソースに検査コードを埋め込む。同じ `.c` を普通に / TSan 付きでコンパイルして、外部参照シンボルを比べると:

```
普通:      printf, pthread_create, pthread_join, __stack_chk_fail
TSan 付き: 上記 + __tsan_func_entry, __tsan_func_exit, __tsan_init,
                 __tsan_read4, __tsan_read8, __tsan_write4
サイズ:    2560 -> 3656 バイト
```

つまり `g = g + 1` が、

```c
__tsan_read4(&g);     // ← 自動挿入
tmp = g;
__tsan_write4(&g);    // ← 自動挿入
g = tmp + 1;
```

のように書き換えられている。

- 再コンパイルが必要
- ネイティブ実行なので速い
- **自分がコンパイルした範囲しか見えない**（libc の中は見えない）
- `-fsanitize=` は「コンパイル時に検査呼び出しを埋め込む」＋「実体が入ったランタイムライブラリをリンクする」の2段構え。**リンク行にも渡さないと `___tsan_write8 not found` になる**（Makefile がリンク行に `$(CFLAGS)` を渡している理由）
- フラグに書く名前は `address` / `thread` / `undefined`。`-fsanitize=asan` は無効（`asan` はあくまで通称）

### Valgrind = 実行時の動的バイナリ変換

CPU を仮想化して全命令を監視する。

- **再コンパイル不要**（`-g` は行番号を出すためだけ）
- 遅い
- **libc の中まで全部見える**。だから `printf` 内部の競合まで報告できる

### 名前の由来

| 略称 | 正式名 | 何を見るか |
|---|---|---|
| ASan | **A**ddress**San**itizer | アドレス（メモリ番地）の使い方 |
| TSan | **T**hread**San**itizer | スレッド間の同期 |
| UBSan | **UB**（Undefined Behavior）**San**itizer | 未定義動作 |
| LSan | **L**eak**San**itizer | リーク（ASan に内包） |
| MSan | **M**emory**San**itizer | 未初期化メモリ（clang 専用、ASan と併用不可） |

### ソースの在処

- Sanitizer: LLVM の compiler-rt — https://github.com/llvm/llvm-project/tree/main/compiler-rt/lib （`asan/` `tsan/` `ubsan/` `lsan/`）
- GCC は上記を取り込んで `libsanitizer/` として同梱 — https://github.com/gcc-mirror/gcc
  （TSan のエラーに出る `src/libsanitizer/tsan/tsan_platform_linux.cpp:296` がこのパス）
- Valgrind: GitHub は非公式ミラーのみ。公式は `git clone https://sourceware.org/git/valgrind.git`

いずれも自分でクローンする必要はない。`gcc` / `valgrind` パッケージに完成品が入っている
（Dockerfile の `libtsan0` / `libasan6` / `libubsan1` がランタイム本体）。

---

## 6. mac で直接やろうとした場合の制約

参考。基本はコンテナを使えばよい。

| | mac (arm64) | Ubuntu |
|---|---|---|
| TSan / ASan / UBSan | 動く | 動く |
| ASan のリーク検出 | **動かない**（`detect_leaks is not supported on this platform`） | 動く |
| valgrind / helgrind | **無い**（brew 必須。arm64 では実質使えない） | 動く |
| gdb | 無い（`lldb` で代替、`thread backtrace all`） | 動く |
| リーク検出の代替 | `MallocStackLogging=1 leaks -atExit -- ./philo ...` | — |
| `pthread_t` の型 | `struct _opaque_pthread_t *`（ポインタ） | `unsigned long` |

最後の行は要注意。`printf("%lu", tid)` は Ubuntu では通るが mac では `-Werror` で落ちる。
デバッグ用の出力は `%p` + `(void *)` キャストにするか、消しておく。

---

## 7. 現在検出されている問題

### リーク 2件（正常終了パス）

`main` が `return (0)` する前に後始末をしていない。

- `philo/init/init.c:42` — `ctx->fork_mutex`
- `philo/init/init.c:61` — `ctx->philo`

エラー処理側は書けている（`init_ctx` は各段階の失敗で前段を後始末している）。
「後始末の関数はあるのに、正常終了パスで呼んでいない」状態。

再現:

```bash
make debug && valgrind --leak-check=full ./philo 5 800 200 200
```

### free_fork_mutex の添字バグ

`philo/init/init.c:93`

```c
pthread_mutex_destroy(fork_mutex);      /* 毎回 fork_mutex[0] を destroy */
```

`i` をインクリメントしているのに添字に使っていないので、`fork_mutex[0]` を n 回破棄して
`[1]` 以降が破棄されない。正しくは `&fork_mutex[i]`。

**これは valgrind でも ASan でも検出されない**（ミューテックスの実体は同じヒープブロック内なので、
リークにも領域外にもならない）。目で読まないと見つからない類。
