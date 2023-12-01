BIN=vk

${BIN}: main.o
	g++ --std=c++17 $^ -o ${BIN} -Os -s -O3 -ldl

main.o: main.cpp
	g++ -c --std=c++17 main.cpp -O3 -g

run: ${BIN}
	./${BIN}

clean:
	rm -f ${BIN} binToTxt comp.spv comp.hpp *.o *.spv
