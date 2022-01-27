BIN=newVar
${BIN}: main.cpp
	g++ main.cpp -std=c++17 -o ${BIN} -pthread

mem: ${BIN}
	valgrind --tool=memcheck --leak-check=yes ./${BIN}

run: ${BIN}
	./${BIN}

clean:
	rm ${BIN}
