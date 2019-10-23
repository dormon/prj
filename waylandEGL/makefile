BIN      = sharedOrNot
LIB      = -lwayland-client -lwayland-server -lwayland-egl -lwayland-cursor -lEGL -lGL
CPPFLAGS = -std=c++1y -Wall -g
CPP      = g++
OBJ      = main.o

all: ${BIN}

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
