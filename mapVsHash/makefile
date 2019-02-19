BIN      = intr
LIB      = 
CPPFLAGS = -std=c++17 -I. -I/media/windata/install/linux/include/ -O2
CPP      = g++-7
OBJ      = main.o sink.o

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
