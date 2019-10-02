YOCTO    = /media/devel/poky/2.7.1/sysroots/aarch64-poky-linux/
BIN      = eglFull
LIB      = -lwayland-client -lEGL -lwayland-cursor -lGL -lwayland-egl -lpthread ${YOCTO}/lib/libm.so.6
CPPFLAGS = -std=c++17 -Wall -O3
CPP      = aarch64-linux-gnu-gcc --sysroot=${YOCTO}
OBJ      = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.c
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
