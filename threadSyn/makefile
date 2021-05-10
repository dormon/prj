BIN=newVar
${BIN}: main.cpp
	g++ main.cpp -std=c++17 -o ${BIN} -pthread

run: ${BIN}
	./${BIN}
