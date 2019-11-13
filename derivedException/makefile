BIN      = isShared
LIB      = -pthread
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o ex.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
