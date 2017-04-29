BIN=lsdir
LSDIRBIN=linuxLsDir.so
GETPROCADDRESSBIN=SDL2GetProcAddress.so
LIB=-ldl -lSDL2

all: ${BIN} ${LSDIRBIN} ${GETPROCADDRESSBIN}

${BIN}: main.cpp LibraryLoader.cpp ObjectLoader.cpp
	g++ -std=c++1y -W $^ -o ${BIN} ${LIB}


${LSDIRBIN}: LinuxLsDir.cpp LsDirInterface.cpp
	g++ -std=c++1y -shared -fPIC $^ -o ${LSDIRBIN} 

${GETPROCADDRESSBIN}: SDL2GetProcAddress.cpp
	g++ -std=c++1y -shared -fPIC $^ -o ${GETPROCADDRESSBIN} -lSDL2

run: ${BIN}
	./${BIN}

clean:
	rm -f *.o ${BIN} ${LSDIRBIN} ${GETPROCADDRESSBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
