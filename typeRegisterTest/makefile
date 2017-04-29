BIN      = typeRegisterTest
LIB      = -lgeCore
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o registrationTest.o arrayRegistrationTest.o structRegistrationTest.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

VALGRINDARGS = --tool=memcheck --leak-check=yes --show-reachable=yes

ARGS00 =

run: ${BIN}
	./${BIN} ${ARGS00}


clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS00}

