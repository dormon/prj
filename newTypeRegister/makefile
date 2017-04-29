BIN      = accessMember
LIB      = 
CPPFLAGS = -std=c++1y -Wall -O3 -I. -g
CPP      = g++-5
OBJ      = main.o geDE/TypeRegister.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o geDE/*.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
