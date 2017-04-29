BIN      = accessMember
LIB      = -lSDL2
CPPFLAGS = -std=c++1y -Wall -O3 -I.
CPP      = g++
OBJ      = main.o geGL/LoadOpenGLFunctions.o geGL/CheckOpenGLFunctions.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o geGL/*.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
