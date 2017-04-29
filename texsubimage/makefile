SOURCEDIRS=app/
#patsubst
#-include ${DEPS}
#-MMD
DIR0:=${SOURCEDIRS}
DIRA0:=${foreach var,${DIR0},${dir ${wildcard ${var}*/}}} ${DIR0}
DIR1:=${sort ${DIRA0}}
DIRA1:=${foreach var,${DIR1},${dir ${wildcard ${var}*/}}} ${DIR1}
DIR2:=${sort ${DIRA1}}
DIRA2:=${foreach var,${DIR2},${dir ${wildcard ${var}*/}}} ${DIR2}
DIR3:=${sort ${DIRA2}}
DIRA3:=${foreach var,${DIR3},${dir ${wildcard ${var}*/}}} ${DIR3}
DIR4:=${sort ${DIRA3}}
SRCDIR=${DIR4}

SOURCE=${foreach var,${SRCDIR},${wildcard ${var}*.cpp}}
HEADER=${foreach var,${SRCDIR},${wildcard ${var}*.hpp}}
OBJ=${SOURCE:%.cpp=%.o}
INCLUDEDIRS=${foreach var,${SRCDIR},-I ${var}}


BIN=svetlo
LIBS      = -lGL -lGLU -lGLEW -lgeGL -lgeUtil -lWindowObject #-Wl,-rpath,/usr/local/lib -g # -lcv -lhighgui
CFLAGS    = -std=c++1y -Wall ${INCLUDEDIRS}
CPP       = g++
#CPP       = wing++
#CPP       = /usr/bin/i586-mingw32msvc-g++

all: ${BIN}

#everything
${BIN}: ${OBJ} makefile
	${CPP} ${OBJ} -o ${BIN} ${LIBS}


#compilation rules
%.o: %.cpp %.hpp
	${CPP} -o $@ ${CFLAGS} -c $< -g

%.o: %.cpp %.h
	${CPP} -o $@ ${CFLAGS} -c $< -g

run:
	./svetlo -w 512 -h 512 --shader-directory shaders/   


clean:
	rm -f ${OBJ} tags ${BIN}

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --suppressions=minimal.supp ./${BIN}
	#valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --error-limit=no --gen-suppressions=all --log-file=minimalraw.log ./${BIN}
