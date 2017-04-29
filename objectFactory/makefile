BIN      = isShared
LIB      = -lgeCore -lgeGL -lWindowObject -lgeUtil -lGL -lGLEW
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o textFileLoader.o fileExtensionExtractor.o fileExtensionTranslator.o shaderObjectFactory.o windowSystem.o callExtensionTranslator.o producer.o registerStdFunctions.o

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
