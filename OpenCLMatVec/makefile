BIN      = intr
LIB      = -lOpenCL -lsqlite3
#CPPFLAGS = -std=c++14 -Wall -O3 -Wconversion -I. -I/usr/local/cuda/include/
CPPFLAGS = -std=c++14 -O3 -Wignored-attributes -I. 
CPP      = g++
OBJ      = main.o CLQuick.o cpuImplementation.o

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
