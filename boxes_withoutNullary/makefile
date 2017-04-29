BIN      = sharedOrNot
LIB      = 
CPPFLAGS = -std=c++14 -Wall -O3
CPP      = g++-5
OBJ      = main.o ArrayDescription.o AtomicDescription.o FceDescription.o Function.o MemFceDescription.o PtrDescription.o Resource.o StructDescription.o TypeDescription.o AnyDescription.o VoidDescription.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

Resource.cpp: Resource.h

main.o: Resource.h keyword.h TypeRegister.h Defines.h

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
