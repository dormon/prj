BIN      = accessMember
LIB      = 
CPPFLAGS = -std=c++14 -Wall -O3
CPP      = g++-5
OBJ      = main.o model.o ModelDescription.o AtomicModelDescription.o VectorModelDescription.o CompositeModelDescription.o CommandDescription.o AtomicCommandDescription.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

main.o: model.h

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
