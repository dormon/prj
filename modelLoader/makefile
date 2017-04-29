BIN      = isShared
LIB      = -lSDL2 -lGLEW -lGL -lgeGL -lassimp
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o pin.o boxInterface.o typeToId.o window.o Scene.o AssimpLoader.o renderModel.o camera.o fpsRotation.o

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
