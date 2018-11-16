all: main.cpp
	g++ --std=c++17 main.cpp TextUtils/Utils.cpp -lOpenCL -o ocl -I. -I/media/data/install/linux2/include -Wno-ignored-attributes

clean:
	rm -rf ocl *.o

run: ocl
	./ocl
