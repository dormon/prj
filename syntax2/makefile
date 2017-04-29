BIN      = accessMember
LIB      = -lgeCore -lgeUtil
CPPFLAGS = -std=c++1y -Wall -O3 -pedantic -W -Wconversion -I.
CPP      = g++
OBJ      = main.o geCore/registerStdFunctions.o geCore/producer.o

all: ${BIN}

${OBJ}: geCore/syntax.h geCore/syntaxTree.h geCore/grammar.h geCore/NodeContext.h

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
