BIN      = sharedOrNot
LIB      = -lncurses -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_video -lopencv_imgcodecs
CPPFLAGS = -std=c++14 -Wall -O3 -fextended-identifiers -pedantic -Wall -W -Wconversion 
CPP      = g++-5
OBJ      = main.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
