BIN=glxgears

all: *.c
	gcc *.c -o ${BIN} -lGL -lX11 -lm -g

clean:
	rm -f ${BIN}

run: ${BIN}
	./${BIN}
