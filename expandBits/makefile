BIN      = isShared
LIB      = -pthread
CPPFLAGS = -std=c++17 -Wall -O3
CPP      = g++-7
OBJ      = main.o\
					 tests.o\
					 shiftDistributiabilityTests.o\
					 expandTests.o\
					 bitTests.o\
					 nofLevelsTests.o\
					 shortestTests.o\
					 middleTests.o\
					 longestTests.o\
					 bitLenTests.o\
					 shortestMaskTests.o
					 


all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
