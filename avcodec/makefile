BIN      = isShared
LIB      = -lavdevice -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o CachedVideo.o




all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h %.hpp CachedVideo.hpp
	${CPP} -o $@ ${CPPFLAGS} -c $<

run: ${BIN}
	./${BIN} /media/papobackup/backup/video/허준/hur-jun-episode-2.mp4 100 2000

clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}
