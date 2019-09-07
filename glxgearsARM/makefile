BIN=glxgears

C      = aarch64-linux-gnu-gcc --sysroot=/media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/

all: *.c
	${C} /media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/lib/libm-2.29.so -lm *.c -o ${BIN} -lm -lpthread -lm -lGL -lm -lX11 -lm -g /media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/lib/libm-2.29.so /media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/lib/libm.so.6

clean:
	rm -f ${BIN}

run: ${BIN}
	./${BIN}
