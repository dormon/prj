BIN      = nestedEnum
LIB      = 
#CPPFLAGS = -std=c++17 -Wall -O3 -DNDEBUG -I/media/data/install/linux2/include
CPPFLAGS = -std=c++17 -Wall -O3
CPP      = g++
OBJ      = main.o print.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
