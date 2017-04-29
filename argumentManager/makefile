BIN      = isShared
LIB      = 
CPPFLAGS = -std=c++1y -Wall -O3
CPP      = g++
OBJ      = main.o ArgumentManager.o ArgData.o ArrayData.o StructData.o BaseData.o

all: ${BIN}

${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIB}

%.o: %.cpp %.h
	${CPP} -o $@ ${CPPFLAGS} -c $<

VALGRINDARGS = --tool=memcheck --leak-check=yes --show-reachable=yes

ARGS00 =
ARGS01 = ahoj float 32.f
ARGS02 = ahoj int 42
ARGS03 = ahoj string dormon
ARGS04 = ahoj [ float 1.2 float 2 ]
ARGS05 = ahoj struct { float 12.3 string bosson }
ARGS06 = mat2 [ [ float 1 float 0 ] [ float 0 float 1 ] ]
ARGS07 = particles [ struct { float 1 string bosson } struct { float -1 string fermion } ]
ARGS08 = soa struct { [ float 1 float 2 ] [ int 0 int 1 ] }
ARGS09 = sos struct { struct { float 1 string lepton } struct { int 0 string meson } }
ARGS10 = particles { bosson float 1 }
ARGS11 = particles { bossons { type string heavy flavor string strange } neutrinos { type string light colour string red } }
ARGS12 = \< args.txt
ARGS13 = shaderDir string \< args2.txt

run: ${BIN}
	./${BIN} ${ARGS00}
	./${BIN} ${ARGS01}
	./${BIN} ${ARGS02}
	./${BIN} ${ARGS03}
	./${BIN} ${ARGS04}
	./${BIN} ${ARGS05}
	./${BIN} ${ARGS06}
	./${BIN} ${ARGS07}
	./${BIN} ${ARGS08}
	./${BIN} ${ARGS09}
	./${BIN} ${ARGS10}
	./${BIN} ${ARGS11}
	./${BIN} ${ARGS12}
	./${BIN} ${ARGS13}



clean:
	rm -f *.o ${BIN} ${LSDIRBIN}

mem: ${BIN}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS00}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS01}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS02}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS03}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS04}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS05}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS06}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS07}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS08}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS09}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS10}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS11}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS12}
	valgrind ${VALGRINDARGS} ./${BIN} ${ARGS13}

