#!/bin/sh

#build libs

mkdir install

cd libs/
mkdir build/
cd build/
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_INSTALL_PREFIX=../../install
make -j4 install

cd ../..

#build apps

cd apps
mkdir build/
cd build/
cmake                                                               \
   ..                                                               \
  -DCMAKE_BUILD_TYPE=DEBUG                                          \
  -DCMAKE_INSTALL_PREFIX=../../install                              \
  -DHeaderOnlyLibrary_DIR=../../install/lib/cmake/HeaderOnlyLibrary
make -j4
