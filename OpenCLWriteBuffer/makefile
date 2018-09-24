BIN      = intr
LIB      = -lOpenCL #/home/dormon/Downloads/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-14.04/lib/libclang.so
CPPFLAGS = -std=c++14 -Wall -O3 -Wconversion
CPP      = g++ #/home/dormon/Downloads/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-14.04/bin/clang
OBJ      = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
