BIN=vk

${BIN}: main.o dormonvu.o
	g++ --std=c++17 $^ -o ${BIN} -Os -s -O3 -ldl

comp.spv: comp.glsl
	glslc -fshader-stage=compute $^ -o comp.spv

binToTxt: binToTxt.cpp
	g++ --std=c++17 binToTxt.cpp -o binToTxt

comp.hpp: comp.spv binToTxt
	echo "uint32_t const code[]={" > comp.hpp
	./binToTxt comp.spv >> comp.hpp
	echo "};" >> comp.hpp


main.o: main.cpp comp.hpp dormonvu.hpp
	g++ -c --std=c++17 main.cpp -O3 -g

dormonvu.o: dormonvu.cpp dormonvu.hpp
	g++ -c --std=c++17 dormonvu.cpp -O3 -g

#%.o: %.cpp
#	g++ -c --std=c++17 $^ -O3 -g


run: ${BIN}
	./${BIN}

clean:
	rm -f ${BIN} binToTxt comp.spv comp.hpp *.o *.spv
