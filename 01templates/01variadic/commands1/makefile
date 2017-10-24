BIN      = accessMember
LIB      = -ldl
CPPFLAGS = -std=c++14 -Wall -O3 -Wconversion
CPP      = g++-5
OBJ      = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN} a b c d light { --position 0 1 2 3 --color 1 0.1 0.01f }

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
