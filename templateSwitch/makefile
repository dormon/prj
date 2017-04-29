BIN      = accessMember
LIB      = 
CPPFLAGS = -std=c++1y -Wall -O3 -pedantic -W -Wconversion
CPP      = g++
OBJ      = main.o

all: ${BIN}

${OBJ}: syntax.h

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
