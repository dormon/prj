BIN       = accessMember
LIB       = -lgeCore
CPPFLAGS = -std=c++1y -Wall -O3 -Wconversion -I.
CC        = g++
OBJ       = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CC} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CC} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
