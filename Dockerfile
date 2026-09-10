# 42 Tokyo (Ubuntu) の評価環境を手元で再現するための開発イメージ。
# 42 のクラスタが 24.04 なら FROM を ubuntu:24.04 に変えるだけでよい。
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
		build-essential \
		clang \
		gdb \
		valgrind \
		libtsan0 \
		libasan6 \
		libubsan1 \
		python3-pip \
		git \
		less \
		vim \
	&& pip3 install --no-cache-dir norminette \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /workspace/philo

CMD ["bash"]
