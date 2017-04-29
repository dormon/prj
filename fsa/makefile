BIN      = fsa
LIB      = -lgeCore
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o fsa.o state.o transition.o tests.o fsa2.o lex2.o state2.o transition2.o range2.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
