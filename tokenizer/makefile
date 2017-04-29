BIN      = sharedOrNot
LIB      = -lgeCore
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o producer.o registerStdFunctions.o asmSyntax.o

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
