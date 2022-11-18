BIN=vk

${BIN}: main.o
	g++ $^ -o ${BIN} -Os -s -O3 -ldl

comp.spv: comp.glsl
	glslc -fshader-stage=compute $^ -o comp.spv

binToTxt: binToTxt.cpp
	g++ binToTxt.cpp -o binToTxt

comp.hpp: comp.spv binToTxt
	echo "uint32_t const code[]={" > comp.hpp
	./binToTxt comp.spv >> comp.hpp
	echo "};" >> comp.hpp

main.o: main.cpp comp.hpp
	g++ -c --std=c++17 main.cpp -O3 -g

#%.o: %.cpp
#	g++ -c --std=c++17 $^ -O3 -g


run: ${BIN}
	./${BIN}

clean:
	rm -f ${BIN} binToTxt comp.spv comp.hpp *.o *.spv
