BIN=vk

${BIN}: main.o devMem.o createShaderModule.o createBuffer.o
	g++ --std=c++17 $^ -o ${BIN} -Os -s -O3 -ldl -lvulkan

%.o: %.cpp %.hpp
	g++ -c --std=c++17 $< -O3 -g -o $@

shader.spv: shader.glsl
	glslang -S comp $^ -V -o shader.spv
	#glslc -fshader-stage=compute $^ -o shader.spv


run: ${BIN} shader.spv
	./${BIN}

clean:
	rm -f ${BIN} binToTxt comp.spv comp.hpp *.o *.spv
