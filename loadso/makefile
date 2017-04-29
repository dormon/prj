BIN=loadso

LIB=-Wl,--export-dynamic -ldl

${BIN}: main.cpp test.so
	g++ -std=c++1y main.cpp -o ${BIN} ${LIB}

test.so: test.cpp
	g++ -std=c++1y test.cpp -shared -rdynamic -fPIC -o test.so

run: ${BIN}
	./${BIN}

mem: ${BIN}
	valgrind ./${BIN}
