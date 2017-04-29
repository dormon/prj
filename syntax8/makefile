BIN      = syntax3
LIB      = -lgeCored -lgeUtild -lgeDEd -lgeParserd
CPPFLAGS = -std=c++1y -Wall -g -O3 -pedantic -W -Wconversion -I.
CC      = g++
OBJ      = main.o #geCore/registerStdFunctions.o geCore/producer.o

all: ${BIN}

${OBJ}:

${BIN}: ${OBJ}
	${CC} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CC} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o geCore/*.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
