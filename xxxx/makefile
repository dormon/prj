BIN=glxgears

all: *.cpp
	g++ *.cpp -o ${BIN} -lGL -lX11 -lm -g -lXi

clean:
	rm -f ${BIN}

run: ${BIN}
	./${BIN}
