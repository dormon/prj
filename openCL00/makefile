all: main.cpp
	g++ --std=c++17 main.cpp TextUtils/Utils.cpp /usr/lib/x86_64-linux-gnu/libOpenCL.so.1 -o ocl -I.

clean:
	rm -rf ocl *.o
