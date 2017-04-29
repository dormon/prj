BIN       = accessMember
LIB       = 
CPPFLAGS = -std=c++14 -Wall -O3 -Wconversion -I. -I/home/dormon/Downloads/boost_1_60_0
CC        = g++-5
OBJ       = main.o

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
