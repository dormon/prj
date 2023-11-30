BIN=spirv_gen

${BIN}: main.o
	g++ --std=c++17 $^ -o ${BIN} 

comp.spv: comp.glsl
	glslang -S comp $^ -V -o comp.spv

main.o: main.cpp
	g++ -c --std=c++17 main.cpp -O3 -g

comp.txt: comp.spv
	spirv-dis comp.spv -o comp.txt

run: ${BIN} comp.txt
	./${BIN}
	spirv-dis cpp_comp.spv -o cpp_comp.txt
	vimdiff comp.txt cpp_comp.txt

clean:
	rm -f ${BIN} *.o *.spv *.txt
