BIN=vk

${BIN}: main.o
	g++ --std=c++17 $^ -o ${BIN} -Os -s -O3 -ldl -lvulkan

main.o: main.cpp
	g++ -c --std=c++17 main.cpp -O3 -g

shader.spv: shader.glsl
	glslang -S comp $^ -V -o shader.spv
	#glslc -fshader-stage=compute $^ -o shader.spv


run: ${BIN} shader.spv
	./${BIN}

clean:
	rm -f ${BIN} binToTxt comp.spv comp.hpp *.o *.spv
