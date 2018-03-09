#!/bin/sh

#build libs
rm -rf build/
mkdir build/
cd build/
cmake .. -DCMAKE_BUILD_TYPE=DEBUG 
make -j4
cd ..

