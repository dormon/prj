WAYLAND=`pkg-config wayland-client --cflags --libs` -lpthread -lEGL -lwayland-egl -lGL
CFLAGS?=-std=c++1z -Wall -Werror -O3 -fvisibility=hidden

CCFLAGS=-std=c++17 -I/media/devel/install/arm/include -L/media/devel/install/arm/lib

CC = g++

YOCTO    = /media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/
CC_ARM = aarch64-linux-gnu-g++ --sysroot=${YOCTO}


BIN_ELF = wayImELF

BIN_ARM = wayImARM
LIB_ARM = -lm-2.29 -lgeGL -ldl

${BIN_ARM}: main.cpp images.bin
	$(CC_ARM) ${CCFLAGS} -o ${BIN_ARM} main.cpp $(WAYLAND) ${LIB_ARM} -lrt

${BIN_ELF}: main.cpp images.bin
	$(CC) ${CCFLAGS} -o ${BIN_ELF} main.cpp $(WAYLAND) -lrt

images.bin: images/convert.py images/window.png images/fish.png
	images/convert.py
	cat window.bin fish.bin > images.bin

clean:
	$(RM) fish.bin window.bin ${BIN_ARM} ${BIN_ELF}

run: ${BIN_ELF}
	./${BIN_ELF}

