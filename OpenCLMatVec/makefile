BIN      = intr
LIB      = -lOpenCL 
CPPFLAGS = -std=c++14 -Wall -O3 -Wconversion -I. -I/usr/local/cuda/include/
CPP      = g++-5
OBJ      = main.o CLQuick.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp CLQuick.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
