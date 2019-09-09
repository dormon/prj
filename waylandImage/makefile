WAYLAND=`pkg-config wayland-client --cflags --libs` -lpthread -lEGL -lwayland-egl
CFLAGS?=-std=c11 -Wall -Werror -O3 -fvisibility=hidden

CC = g++
BIN = hello_wayland
${BIN}: main.cpp images.bin
	$(CC) -o ${BIN} main.cpp $(WAYLAND) -lrt

images.bin: images/convert.py images/window.png images/fish.png
	images/convert.py
	cat window.bin fish.bin > images.bin

clean:
	$(RM) fish.bin window.bin hello_wayland

run: ${BIN}
	./${BIN}

