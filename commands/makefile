BIN      = accessMember
LIB      = -lgeCored -lgeGLd -ldl
CPPFLAGS = -std=c++1y -Wall -O3 -Wconversion
CPP      = g++
OBJ      = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN} a b c d light { --position 0 1 2 3 --color 1 0.1 0.01f }

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
