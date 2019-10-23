all: main.cpp
	aarch64-linux-gnu-g++ main.cpp --sysroot=/media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/ -lwayland-client -pthread -lEGL -lGL -lwayland-server -lwayland-cursor -lm-2.29 -lwayland-egl ; scp a.out root@192.168.0.1:/home/root
run: all
	ssh -t root@192.168.0.1 "XDG_RUNTIME_DIR=/run/user/0 /home/root/a.out"
