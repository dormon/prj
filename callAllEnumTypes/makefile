BIN=callAll
LSDIRBIN=
GETPROCADDRESSBIN=
LIB=

all: ${BIN}

${BIN}: main.cpp
	g++ -std=c++17 -W $^ -o ${BIN} ${LIB}


run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
