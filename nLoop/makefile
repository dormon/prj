BIN      = referenceWrapper
LIB      = #-lgcov
#CPPFLAGS = -std=c++14 -Wall -O3 #-fprofile-use=/home/dormon/git/prj/nLoop/main.gcda
CPPFLAGS = -std=c++14 -Wall -O3 -fprofile-use=/home/dormon/git/prj/nLoop/main.gcda -fprofile-use=/home/dormon/git/prj/nLoop/forBody.gcda
#CPPFLAGS = -std=c++14 -Wall -O3 -fprofile-generate
CC       = g++-5
OBJ      = main.o forBody.o

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
