WAYLAND=`pkg-config wayland-client --cflags --libs` -lpthread
CFLAGS?=-std=c11 -Wall -Werror -O3 -fvisibility=hidden

CC = gcc
BIN = hello_wayland
${BIN}: main.c helpers.c helpers.h images.bin
	$(CC) -o ${BIN} *.c $(WAYLAND) -lrt

images.bin: images/convert.py images/window.png images/fish.png
	images/convert.py
	cat window.bin fish.bin > images.bin

clean:
	$(RM) fish.bin window.bin hello_wayland

run: ${BIN}
	./${BIN}

