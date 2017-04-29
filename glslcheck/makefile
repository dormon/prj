SOURCEDIRS=src/
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
LIBS      = -lGLU -lSDL2 -lGLEW -lassimp -lAntTweakBar -lgeGL -lgeUtil -lgeCore -lGPUPerfAPIGL -lAntTweakBar -lIL -lILU -lILUT -lGL -g -O3
CFLAGS    = -std=c++1y -Wall -O3 ${INCLUDEDIRS}
CPP       = g++
#CPP       = wing++
#CPP       = /usr/bin/i586-mingw32msvc-g++

all: ${BIN}

#everything
${BIN}: ${OBJ}
	${CPP} ${OBJ} -o ${BIN} ${LIBS}


#compilation rules
%.o: %.cpp %.hpp
	${CPP} -o $@ ${CFLAGS} -c $< -g

%.o: %.cpp %.h
	${CPP} -o $@ ${CFLAGS} -c $< -g

WIDTH=1024
HEIGHT=1024
MODEL=/home/dormon/Plocha/twotessellatedquads/twotessellatedquads.obj
#MODEL=/home/dormon/Plocha/o/o.3ds
#MODEL=/home/dormon/Plocha/ot/o.obj
#MODEL=/home/dormon/Plocha/sponza/sponza.obj




run:
	#./svetlo -w 1024 -h 768 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -d -fi -r -c --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 4096 --shadowmap-end -m ~/Plocha/sponza/sponza.obj --test-measure-compute
	#./svetlo -w 512 -h 512 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -d -fi -r -c --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 4096 --shadowmap-end -m ~/Plocha/2tri/2tri.3ds --test-measure-compute
	#./svetlo -w 1024 -h 1024 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -u -d -fi -c -r --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 512 -fpx -3 -fpy 0.5 -fpz -3 --shadowmap-end -m /home/dormon/Plocha/o/o.3ds --test-measure-compute
	#./svetlo -w ${WIDTH} -h ${HEIGHT} --shader-directory shaders/ --light-start 1.8 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -u -d -fi -c -r --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 512 -fpx -3 -fpy 0.5 -fpz -3 --shadowmap-end -m ${MODEL} --test-measure-compute
	./svetlo \< window.conf \< context.conf \< light.conf \< test0.conf \< methods.conf


gdb:
	gdbtui ./svetlo -w 512 -h 512 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -u -d -fi -c -r --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 512 -fpx -3 -fpy 0.5 -fpz -3 --shadowmap-end -m /home/dormon/Plocha/o/o.3ds --test-measure-compute



	#./svetlo --shader-directory shaders/ -m ~/Plocha/sibenik/sibenik.obj --test --test-cameramation sibenikprulet --test-method cs --compute-start -c --compute-end --light-start 0 -15 0 1 1 1 1 1 1 1 --light-end


clean:
	rm -f ${OBJ} tags ${BIN}

mem: ${BIN}
	#valgrind --tool=memcheck --leak-check=yes --suppressions=minimal.sup ./${BIN} -w 512 -h 512 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -u -d -fi -c -r --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 1024 -fpx -3 -fpy 0.5 -fpz -3 --shadowmap-end -m /home/dormon/Plocha/o/o.3ds --test-measure-compute 2> memleak
	valgrind --tool=memcheck --leak-check=yes ./${BIN} window \{ size ${WIDTH} ${HEIGHT} \} \< context.conf \< light.conf \< test0.conf \< methods.conf
	#valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --suppressions=minimal.sup --gen-suppressions=all ./${BIN} -w 512 -h 512 --shader-directory shaders/ --light-start 0 0 0 1 1 1 1 1 1 1 --light-end  --test-duration 60 --test-method sm --computesoe-start -c --computesoe-end --compute-start -c --compute-end -d -r -fi --geometry-start -u -d -fi -c -r --geometry-end --tessellation-start -r -sve --tessellation-end --shadowmap-start -r 1024 -fpx -3 -fpy 0.5 -fpz -3 --shadowmap-end -m /home/dormon/Plocha/o/o.3ds --test-measure-compute



	#valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --error-limit=no --gen-suppressions=all --log-file=minimalraw.log ./${BIN}
