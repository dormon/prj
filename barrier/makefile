BIN      = intr
LIB      = 
CPPFLAGS = -std=c++17 -Wconversion -O3 -I. -I/media/windata/git/Shadows/install/include -L/media/windata/git/Shadows/install/lib -lVars
CPP      = g++-7
OBJ      = main.o

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
